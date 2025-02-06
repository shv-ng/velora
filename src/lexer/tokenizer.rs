use lazy_static::lazy_static;
use regex::Regex;

use super::token::Token;

lazy_static! {
    static ref RE: Regex = Regex::new(r"[()]|[+\-*/]|\d+\.\d+|\d+|[^\s]+").unwrap();
    static ref INT_RE: Regex = Regex::new(r"\d+").unwrap();
    static ref FLOAT_RE: Regex = Regex::new(r"\d+\.\d+").unwrap();
}

fn is_int(val: &str) -> bool {
    if is_float(val) {
        return false;
    };
    INT_RE.is_match(val)
}

fn is_float(val: &str) -> bool {
    FLOAT_RE.is_match(val)
}

pub fn tokenize(input: &str) -> Result<Vec<Token>, String> {
    let mut tokens = Vec::new();

    let mut line_number = 1;
    let mut column_number;

    for (_, line) in input.lines().enumerate() {
        for token in RE.find_iter(line) {
            column_number = token.start();

            let token_str = token.as_str();
            match classify_token(token_str) {
                Ok(t) => tokens.push(t),
                Err(e) => {
                    return Err(format!(
                        "Error at line {}, column {}: {}",
                        line_number, column_number, e
                    ))
                }
            }
        }
        line_number += 1;
    }

    Ok(tokens)
}

fn classify_token(token: &str) -> Result<Token, String> {
    if is_float(token) {
        return Ok(Token::Float(token.parse().unwrap()));
    }
    if is_int(token) {
        return Ok(Token::Int(token.parse().unwrap()));
    }
    match token {
        "+" => Ok(Token::Plus),
        "-" => Ok(Token::Minus),
        "*" => Ok(Token::Multiply),
        "/" => Ok(Token::Divide),
        "(" => Ok(Token::LParan),
        ")" => Ok(Token::RParan),
        _ => Err(format!("Unknown token: {}", token)),
    }
}
