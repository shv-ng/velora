use crate::lexer::token::Token;

use super::expression::{BinaryOp, Operator};

pub fn parse(tokens: Vec<Token>) -> Result<BinaryOp, String> {
    if tokens.is_empty() {
        return Err("Expected Number".to_string());
    }
    let mut iter = tokens.iter();

    let mut left = parse_number(iter.next()).unwrap();
    while let Some(op) = iter.next() {
        let operator = match op {
            Token::Plus => Operator::Plus,
            Token::Minus => Operator::Minus,
            Token::Multiply => Operator::Multiply,
            Token::Divide => Operator::Divide,
            _ => return Err("Unexpected Operator".to_string()),
        };
        let right = parse_number(iter.next()).unwrap();

        left = BinaryOp::BinaryOp {
            left: Box::new(left),
            op: operator,
            right: Box::new(right),
        };
    }
    Ok(left)
}

fn parse_number(token: Option<&Token>) -> Result<BinaryOp, String> {
    if let Some(val) = token {
        match val {
            Token::Int(value) => Ok(BinaryOp::Number(super::expression::Number::Int(*value))),
            Token::Float(value) => Ok(BinaryOp::Number(super::expression::Number::Float(*value))),
            _ => return Err("Expected Number".to_string()),
        }
    } else {
        Err("Expected Number".to_string())
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::{lexer::token::Token, parser::expression::Number};

    // Test valid cases
    #[test]
    fn test_parse_addition() {
        let tokens = vec![Token::Int(3), Token::Plus, Token::Int(4)];
        let result = parse(tokens);
        let expected = BinaryOp::BinaryOp {
            left: Box::new(BinaryOp::Number(Number::Int(3))),
            op: Operator::Plus,
            right: Box::new(BinaryOp::Number(Number::Int(4))),
        };
        assert_eq!(result, Ok(expected));
    }

    #[test]
    fn test_parse_subtraction() {
        let tokens = vec![Token::Int(5), Token::Minus, Token::Int(2)];
        let result = parse(tokens);
        let expected = BinaryOp::BinaryOp {
            left: Box::new(BinaryOp::Number(Number::Int(5))),
            op: Operator::Minus,
            right: Box::new(BinaryOp::Number(Number::Int(2))),
        };
        assert_eq!(result, Ok(expected));
    }

    #[test]
    fn test_parse_multiplication() {
        let tokens = vec![Token::Int(2), Token::Multiply, Token::Int(3)];
        let result = parse(tokens);
        let expected = BinaryOp::BinaryOp {
            left: Box::new(BinaryOp::Number(Number::Int(2))),
            op: Operator::Multiply,
            right: Box::new(BinaryOp::Number(Number::Int(3))),
        };
        assert_eq!(result, Ok(expected));
    }

    #[test]
    fn test_parse_division() {
        let tokens = vec![Token::Int(6), Token::Divide, Token::Int(2)];
        let result = parse(tokens);
        let expected = BinaryOp::BinaryOp {
            left: Box::new(BinaryOp::Number(Number::Int(6))),
            op: Operator::Divide,
            right: Box::new(BinaryOp::Number(Number::Int(2))),
        };
        assert_eq!(result, Ok(expected));
    }

    // Test edge cases
    #[test]
    fn test_parse_single_number() {
        let tokens = vec![Token::Int(7)];
        let result = parse(tokens);
        let expected = BinaryOp::Number(Number::Int(7));
        assert_eq!(result, Ok(expected));
    }

    #[test]
    fn test_parse_empty_tokens() {
        let tokens: Vec<Token> = Vec::new();
        let result = parse(tokens);
        assert_eq!(result, Err("Expected Number".to_string()));
    }

    // Test invalid cases
    #[test]
    fn test_parse_invalid_operator() {
        let tokens = vec![
            Token::Int(3),
            Token::Plus,
            Token::Int(4),
            Token::Multiply, // This should raise an error
            Token::Int(5),
        ];
        let result = parse(tokens);
        assert_eq!(result, Err("Unexpected Operator".to_string()));
    }

    #[test]
    fn test_parse_missing_operand() {
        let tokens = vec![
            Token::Int(3),
            Token::Plus,
            // No second operand here
        ];
        let result = parse(tokens);
        assert_eq!(result, Err("Expected Number".to_string()));
    }
}
