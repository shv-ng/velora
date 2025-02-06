use crate::lexer::token::Token;

#[derive(Debug, PartialEq, Clone)]
pub enum BinaryOp {
    Number(Number),
    BinaryOp {
        op: Operator,
        left: Box<BinaryOp>,
        right: Box<BinaryOp>,
    },
}

#[derive(Debug, PartialEq, Clone)]
pub enum Operator {
    Plus,
    Minus,
    Multiply,
    Divide,
}

impl Operator {
    pub fn from_token(token: &Token) -> Option<Operator> {
        match token {
            Token::Plus => Some(Operator::Plus),
            Token::Minus => Some(Operator::Minus),
            Token::Multiply => Some(Operator::Multiply),
            Token::Divide => Some(Operator::Divide),
            _ => None,
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq)]
pub enum Number {
    Int(i64),
    Float(f64),
}

impl BinaryOp {
    pub fn evaluate(&self) -> Number {
        match self {
            BinaryOp::Number(n) => *n,
            BinaryOp::BinaryOp { left, op, right } => {
                let left_value = left.evaluate();
                let right_value = right.evaluate();

                match op {
                    Operator::Plus => match (left_value, right_value) {
                        (Number::Int(l), Number::Int(r)) => Number::Int(l + r),
                        (Number::Int(l), Number::Float(r)) => Number::Float(l as f64 + r),
                        (Number::Float(l), Number::Int(r)) => Number::Float(l + r as f64),
                        (Number::Float(l), Number::Float(r)) => Number::Float(l + r),
                    },
                    Operator::Minus => match (left_value, right_value) {
                        (Number::Int(l), Number::Int(r)) => Number::Int(l - r),
                        (Number::Int(l), Number::Float(r)) => Number::Float(l as f64 - r),
                        (Number::Float(l), Number::Int(r)) => Number::Float(l - r as f64),
                        (Number::Float(l), Number::Float(r)) => Number::Float(l - r),
                    },
                    Operator::Multiply => match (left_value, right_value) {
                        (Number::Int(l), Number::Int(r)) => Number::Int(l * r),
                        (Number::Int(l), Number::Float(r)) => Number::Float(l as f64 * r),
                        (Number::Float(l), Number::Int(r)) => Number::Float(l * r as f64),
                        (Number::Float(l), Number::Float(r)) => Number::Float(l * r),
                    },
                    Operator::Divide => match (left_value, right_value) {
                        (Number::Int(l), Number::Int(r)) => Number::Int(l / r),
                        (Number::Int(l), Number::Float(r)) => Number::Float(l as f64 / r),
                        (Number::Float(l), Number::Int(r)) => Number::Float(l / r as f64),
                        (Number::Float(l), Number::Float(r)) => Number::Float(l / r),
                    },
                }
            }
        }
    }
}
impl Number {
    pub fn as_float(&self) -> f64 {
        match self {
            Number::Int(n) => *n as f64,
            Number::Float(n) => *n,
        }
    }
    pub fn is_int(&self) -> bool {
        match self {
            Number::Int(_) => true,
            Number::Float(_) => false,
        }
    }
    pub fn as_int(&self) -> i64 {
        match self {
            Number::Int(n) => *n,
            Number::Float(n) => *n as i64,
        }
    }
}
