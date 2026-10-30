use crate::Result;
use serde::Deserialize;
use serde_json::{Value, json};
use std::ffi::CString;
use std::io::{Read, Seek, SeekFrom};
use std::os::fd::AsRawFd;
use std::os::unix::ffi::OsStringExt;
use std::time::{Duration, Instant};

unsafe extern "C" {
    fn posix_spawn_file_actions_addinherit_np(
        actions: *mut libc::posix_spawn_file_actions_t,
        fd: libc::c_int,
    ) -> libc::c_int;
}

#[derive(Deserialize)]
pub struct Request {
    program: String,
    arguments: Vec<String>,
    root: String,
    timeout_ms: u64,
}

fn check(code: libc::c_int) -> std::io::Result<()> {
    if code == 0 {
        Ok(())
    } else {
        Err(std::io::Error::from_raw_os_error(code))
    }
}

struct Attributes(libc::posix_spawnattr_t);
impl Drop for Attributes {
    fn drop(&mut self) {
        unsafe { libc::posix_spawnattr_destroy(&mut self.0) };
    }
}

struct Actions(libc::posix_spawn_file_actions_t);
impl Drop for Actions {
    fn drop(&mut self) {
        unsafe { libc::posix_spawn_file_actions_destroy(&mut self.0) };
    }
}

struct Child {
    pid: libc::pid_t,
    active: bool,
}

impl Child {
    fn wait(&mut self, options: libc::c_int) -> std::io::Result<Option<i32>> {
        let mut status = 0;
        loop {
            let result = unsafe { libc::waitpid(self.pid, &mut status, options) };
            if result == self.pid {
                self.active = false;
                return Ok(Some(if libc::WIFEXITED(status) {
                    libc::WEXITSTATUS(status)
                } else {
                    -libc::WTERMSIG(status)
                }));
            }
            if result == 0 {
                return Ok(None);
            }
            let error = std::io::Error::last_os_error();
            if error.raw_os_error() == Some(libc::ECHILD) {
                self.active = false;
            }
            if error.kind() != std::io::ErrorKind::Interrupted {
                return Err(error);
            }
        }
    }
}

impl Drop for Child {
    fn drop(&mut self) {
        if self.active {
            unsafe { libc::kill(self.pid, libc::SIGKILL) };
            let _ = self.wait(0);
        }
    }
}

pub fn run(request: Request) -> Result<Value> {
    let program = CString::new(request.program)?;
    let root = CString::new(request.root)?;
    let arguments: Vec<_> = std::iter::once(Ok(program.clone()))
        .chain(request.arguments.into_iter().map(CString::new))
        .collect::<std::result::Result<_, _>>()?;
    let environment: Vec<_> = std::env::vars_os()
        .map(|(key, value)| {
            let mut entry = key.into_vec();
            entry.push(b'=');
            entry.extend(value.into_vec());
            CString::new(entry)
        })
        .collect::<std::result::Result<_, _>>()?;
    let pointers = |values: &[CString]| {
        values
            .iter()
            .map(|value| value.as_ptr().cast_mut())
            .chain(std::iter::once(std::ptr::null_mut()))
            .collect::<Vec<_>>()
    };
    let argv = pointers(&arguments);
    let envp = pointers(&environment);
    let mut stdout = tempfile::tempfile()?;
    let mut stderr = tempfile::tempfile()?;
    // CLOEXEC_DEFAULT closes unrelated descriptors without Python's descriptor scan.
    let mut process = unsafe {
        let mut attributes = std::mem::zeroed();
        check(libc::posix_spawnattr_init(&mut attributes))?;
        let mut attributes = Attributes(attributes);
        check(libc::posix_spawnattr_setflags(
            &mut attributes.0,
            (libc::POSIX_SPAWN_CLOEXEC_DEFAULT | libc::POSIX_SPAWN_SETSIGDEF) as _,
        ))?;
        let mut signals = std::mem::zeroed();
        check(libc::sigemptyset(&mut signals))?;
        check(libc::sigaddset(&mut signals, libc::SIGPIPE))?;
        check(libc::sigaddset(&mut signals, libc::SIGXFSZ))?;
        check(libc::posix_spawnattr_setsigdefault(
            &mut attributes.0,
            &signals,
        ))?;
        let mut actions = std::mem::zeroed();
        check(libc::posix_spawn_file_actions_init(&mut actions))?;
        let mut actions = Actions(actions);
        check(libc::posix_spawn_file_actions_addchdir_np(
            &mut actions.0,
            root.as_ptr(),
        ))?;
        if libc::fcntl(0, libc::F_GETFD) >= 0 {
            check(posix_spawn_file_actions_addinherit_np(&mut actions.0, 0))?;
        }
        check(libc::posix_spawn_file_actions_adddup2(
            &mut actions.0,
            stdout.as_raw_fd(),
            1,
        ))?;
        check(libc::posix_spawn_file_actions_adddup2(
            &mut actions.0,
            stderr.as_raw_fd(),
            2,
        ))?;
        let mut pid = 0;
        check(libc::posix_spawn(
            &mut pid,
            program.as_ptr(),
            &actions.0,
            &attributes.0,
            argv.as_ptr(),
            envp.as_ptr(),
        ))?;
        Child { pid, active: true }
    };
    let started = Instant::now();
    let mut timed_out = false;
    let returncode = loop {
        if let Some(code) = process.wait(libc::WNOHANG)? {
            break code;
        }
        if started.elapsed() >= Duration::from_millis(request.timeout_ms) {
            timed_out = true;
            unsafe { libc::kill(process.pid, libc::SIGKILL) };
            break process.wait(0)?.ok_or("Compiler was not reaped")?;
        }
        std::thread::sleep(Duration::from_millis(1));
    };
    let mut out = Vec::new();
    let mut err = Vec::new();
    stdout.seek(SeekFrom::Start(0))?;
    stderr.seek(SeekFrom::Start(0))?;
    stdout.read_to_end(&mut out)?;
    stderr.read_to_end(&mut err)?;
    Ok(json!({"returncode":returncode,"stdout":out,"stderr":err,"timed_out":timed_out}))
}
