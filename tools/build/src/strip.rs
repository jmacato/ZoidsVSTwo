fn is_ident(byte: u8) -> bool {
    byte.is_ascii_alphanumeric() || byte == b'_'
}

fn tokens_at(text: &[u8], word: &[u8]) -> Vec<usize> {
    let mut found = Vec::new();
    let mut index = 0;
    while index + word.len() <= text.len() {
        if &text[index..index + word.len()] == word
            && (index == 0 || !is_ident(text[index - 1]))
            && text.get(index + word.len()).is_none_or(|&b| !is_ident(b))
        {
            found.push(index);
            index += word.len();
        } else {
            index += 1;
        }
    }
    found
}

pub fn mentions_mwcc_asm(source: &str) -> bool {
    let bytes = source.as_bytes();
    !tokens_at(bytes, b"asm").is_empty() || source.contains("ASM_")
}

fn has_asm_block(body: &[u8]) -> bool {
    tokens_at(body, b"asm").into_iter().any(|start| {
        body[start + 3..]
            .iter()
            .find(|b| !b.is_ascii_whitespace())
            .is_some_and(|&b| b == b'{')
    })
}

fn skip_literal(text: &[u8], start: usize) -> usize {
    let quote = text[start];
    let mut index = start + 1;
    while index < text.len() && text[index] != quote {
        if text[index] == b'\\' {
            index += 1;
        }
        index += 1;
    }
    index + 1
}

fn matching_brace(text: &[u8], open: usize) -> Option<usize> {
    let mut depth = 0usize;
    let mut index = open;
    while index < text.len() {
        match text[index] {
            b'"' | b'\'' => {
                index = skip_literal(text, index);
                continue;
            }
            b'{' => depth += 1,
            b'}' => {
                depth -= 1;
                if depth == 0 {
                    return Some(index);
                }
            }
            _ => {}
        }
        index += 1;
    }
    None
}

fn clean_header(header: &str) -> String {
    header
        .lines()
        .filter(|line| !line.trim_start().starts_with('#'))
        .collect::<Vec<_>>()
        .join("\n")
}

fn remove_asm_keyword(header: &str) -> String {
    let mut output = header.to_owned();
    for start in tokens_at(header.as_bytes(), b"asm").into_iter().rev() {
        output.replace_range(start..start + 3, "");
    }
    output
}

pub fn strip_mwcc_asm(text: &str) -> String {
    let bytes = text.as_bytes();
    let mut output = String::with_capacity(text.len());
    let mut copied = 0;
    let mut declaration = 0;
    let mut scopes: Vec<bool> = Vec::new();
    let mut index = 0;
    let mut line_start = true;
    while index < bytes.len() {
        let byte = bytes[index];
        if line_start && byte == b'#' {
            let leading = text[declaration..index].trim().is_empty();
            while index < bytes.len() && bytes[index] != b'\n' {
                index += 1;
            }
            if leading {
                declaration = index;
            }
            continue;
        }
        line_start = byte == b'\n';
        let opaque = scopes.iter().filter(|&&transparent| !transparent).count();
        match byte {
            b'"' | b'\'' => {
                index = skip_literal(bytes, index);
                continue;
            }
            b';' if opaque == 0 => declaration = index + 1,
            b'{' if opaque == 0 => {
                let header = clean_header(&text[declaration..index]);
                let trimmed = header.trim();
                if trimmed.ends_with("extern \"C\"") || trimmed.starts_with("namespace") {
                    scopes.push(true);
                    declaration = index + 1;
                } else if (trimmed.ends_with(')') || trimmed.ends_with("const"))
                    && !trimmed.contains('=')
                    && let Some(end) = matching_brace(bytes, index)
                {
                    let header_has_asm = !tokens_at(trimmed.as_bytes(), b"asm").is_empty();
                    if header_has_asm || has_asm_block(&bytes[index..end]) {
                        output.push_str(&text[copied..declaration]);
                        output.push('\n');
                        output.push_str(remove_asm_keyword(trimmed).trim());
                        output.push_str(";\n");
                        let newlines = text[declaration..=end].matches('\n').count();
                        output.push_str(&"\n".repeat(newlines.saturating_sub(2)));
                        copied = end + 1;
                    }
                    index = end + 1;
                    declaration = index;
                    continue;
                } else {
                    scopes.push(false);
                }
            }
            b'{' => scopes.push(false),
            b'}' => {
                if let Some(true) = scopes.pop() {
                    declaration = index + 1;
                }
            }
            _ => {}
        }
        index += 1;
    }
    output.push_str(&text[copied..]);
    output
}
