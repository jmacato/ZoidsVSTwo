fn take_length(text: &str) -> Option<(&str, &str)> {
    let digits = text.bytes().take_while(u8::is_ascii_digit).count();
    let length: usize = text.get(..digits)?.parse().ok()?;
    let rest = &text[digits..];
    Some((rest.get(..length)?, &rest[length..]))
}

fn parameters(text: &str) -> Option<String> {
    let mut output = String::new();
    let mut rest = text;
    while let Some(c) = rest.chars().next() {
        if c.is_ascii_digit() {
            let (name, after) = take_length(rest)?;
            output.push_str(&format!("{}{name}", name.len()));
            rest = after;
            continue;
        }
        rest = &rest[1..];
        output.push_str(match c {
            'v' => "v",
            'b' => "b",
            'c' => "c",
            'a' => "Sc",
            'h' => "Uc",
            's' => "s",
            't' => "Us",
            'i' => "i",
            'j' => "Ui",
            'l' => "l",
            'm' => "Ul",
            'x' => "x",
            'y' => "Ux",
            'f' => "f",
            'd' => "d",
            'P' => "P",
            'K' => "C",
            'R' => "R",
            _ => return None,
        });
    }
    Some(output)
}

pub fn local_name(name: &str) -> String {
    if let Some(rest) = name.strip_prefix("_ZL")
        && let Some((identifier, _)) = take_length(rest)
    {
        return local_name(identifier);
    }
    if let Some(number) = name.strip_prefix("lit_")
        && !number.is_empty()
        && number.bytes().all(|b| b.is_ascii_digit())
    {
        return format!("@{number}");
    }
    name.to_owned()
}

pub fn mwcc_names(name: &str) -> Vec<String> {
    let mut names = Vec::new();
    if name.starts_with("_ZL") || name.starts_with("lit_") {
        names.push(local_name(name));
    } else if let Some(rest) = name.strip_prefix("_ZN") {
        let (constant, rest) = match rest.strip_prefix('K') {
            Some(rest) => (true, rest),
            None => (false, rest),
        };
        if let Some((class, rest)) = take_length(rest)
            && let Some((member, rest)) = take_length(rest)
            && let Some(rest) = rest.strip_prefix('E')
        {
            let class = format!("{}{class}", class.len());
            if rest.is_empty() {
                names.push(format!("{member}__{class}"));
            } else if let Some(params) = parameters(rest) {
                let qualifier = if constant { "C" } else { "" };
                names.push(format!("{member}__{class}{qualifier}F{params}"));
            }
        }
    } else if let Some(rest) = name.strip_prefix("_Z")
        && let Some((function, rest)) = take_length(rest)
        && let Some(params) = parameters(rest)
    {
        names.push(format!("{function}__F{params}"));
    }
    names
}

pub fn static_local(name: &str) -> Option<(String, String)> {
    if let Some(rest) = name.strip_prefix("_ZZ") {
        let function = take_length(rest)?.0.to_owned();
        for (index, _) in rest
            .match_indices('E')
            .collect::<Vec<_>>()
            .into_iter()
            .rev()
        {
            if let Some((variable, tail)) = take_length(&rest[index + 1..])
                && (tail.is_empty() || tail.starts_with('_'))
            {
                return Some((function, variable.to_owned()));
            }
        }
        return None;
    }
    let (function, variable) = name.split_once('.')?;
    let variable = variable.split('.').next()?;
    (!variable.is_empty() && !function.is_empty())
        .then(|| (function.to_owned(), variable.to_owned()))
}
