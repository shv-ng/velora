#[derive(Debug, PartialEq)]
pub enum BinaryOp {
    Number(Number),
    BinaryOp {
        left: Box<BinaryOp>,
        op: Operator,
        right: Box<BinaryOp>,
    },
}

#[derive(Debug, PartialEq)]
pub enum Operator {
    Plus,
    Minus,
    Multiply,
    Divide,
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
