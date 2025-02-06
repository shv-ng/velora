use lazy_static::lazy_static;
use regex::Regex;

use super::token::Token;

lazy_static! {
    static ref RE: Regex = Regex::new(r"[+\-*/]|\d+\.\d+|\d+|[^\s]+").unwrap();
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
        _ => Err(format!("Unknown token: {}", token)),
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_tokenize_valid_input() {
        let input = "10 + 2.5 * 3";
        let result = tokenize(input);

        assert!(result.is_ok());
        let tokens = result.unwrap();

        assert_eq!(tokens.len(), 5); // Should have 5 tokens: [Int(10), Plus, Float(2.5), Multiply, Int(3)]

        assert_eq!(tokens[0], Token::Int(10));
        assert_eq!(tokens[1], Token::Plus);
        assert_eq!(tokens[2], Token::Float(2.5));
        assert_eq!(tokens[3], Token::Multiply);
        assert_eq!(tokens[4], Token::Int(3));
    }

    #[test]
    fn test_tokenize_invalid_token() {
        let input = "10 + x";
        let result = tokenize(input);

        assert!(result.is_err());
        assert_eq!(
            result.unwrap_err(),
            "Error at line 1, column 5: Unknown token: x"
        );
    }

    #[test]
    fn test_tokenize_empty_input() {
        let input = "";
        let result = tokenize(input);

        assert!(result.is_ok());
        let tokens = result.unwrap();

        assert_eq!(tokens.len(), 0); // Empty input should result in an empty token list
    }

    #[test]
    fn test_tokenize_single_token() {
        let input = "42";
        let result = tokenize(input);

        assert!(result.is_ok());
        let tokens = result.unwrap();

        assert_eq!(tokens.len(), 1);
        assert_eq!(tokens[0], Token::Int(42)); // Single token should be an integer token
    }

    #[test]
    fn test_is_int() {
        assert!(is_int("10"));
        assert!(!is_int("2.5"));
        assert!(!is_int("a"));
    }

    #[test]
    fn test_is_float() {
        assert!(is_float("2.5"));
        assert!(!is_float("10"));
        assert!(!is_float("a"));
    }

    #[test]
    fn test_classify_token() {
        assert_eq!(classify_token("10"), Ok(Token::Int(10)));
        assert_eq!(classify_token("2.5"), Ok(Token::Float(2.5)));
        assert_eq!(classify_token("+"), Ok(Token::Plus));
        assert_eq!(classify_token("-"), Ok(Token::Minus));
        assert_eq!(classify_token("*"), Ok(Token::Multiply));
        assert_eq!(classify_token("/"), Ok(Token::Divide));
        assert_eq!(classify_token("a"), Err("Unknown token: a".to_string()));
    }
}
