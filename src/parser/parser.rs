use crate::lexer::token::Token;

use super::expression::{BinaryOp, Number, Operator};

pub fn parse(tokens: Vec<Token>) -> Result<BinaryOp, String> {
    if tokens.is_empty() {
        return Err("Expected Number".to_string());
    }
    let mut iter = tokens.iter();

    let mut output = Vec::new();
    let mut stack = Vec::new();

    while let Some(token) = iter.next() {
        match token {
            Token::Int(_) | Token::Float(_) => output.push(parse_number(Some(token))?),
            Token::Plus | Token::Minus | Token::Multiply | Token::Divide => {
                while let Some(op) = stack.last() {
                    if precedence(op) >= precedence(token) {
                        let right = output.pop().unwrap();
                        let left = output
                            .pop()
                            .unwrap_or_else(|| BinaryOp::Number(Number::Int(0)));
                        let op = Operator::from_token(&stack.pop().unwrap()).unwrap();

                        output.push(BinaryOp::BinaryOp {
                            left: Box::new(left),
                            op,
                            right: Box::new(right),
                        });
                    } else {
                        break;
                    }
                }
                stack.push(token.clone());
            }
            Token::LParan => {
                stack.push(token.clone());
            }
            Token::RParan => {
                while let Some(op) = stack.pop() {
                    if op == Token::LParan {
                        break;
                    } else {
                        let right = output.pop().unwrap();
                        let left = output.pop().unwrap();
                        let op = Operator::from_token(&op).unwrap();
                        output.push(BinaryOp::BinaryOp {
                            left: Box::new(left),
                            op,
                            right: Box::new(right),
                        });
                    }
                }
            }
        }
    }
    while let Some(op) = stack.pop() {
        let right = output.pop().unwrap();
        let left = output.pop().unwrap();
        let op = Operator::from_token(&op).unwrap();
        output.push(BinaryOp::BinaryOp {
            op,
            left: Box::new(left),
            right: Box::new(right),
        });
    }

    Ok(output.pop().unwrap())
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

fn precedence(token: &Token) -> u32 {
    match token {
        Token::Plus | Token::Minus => 1,
        Token::Multiply | Token::Divide => 2,
        _ => 0,
    }
}
