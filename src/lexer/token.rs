#[derive(Debug, PartialEq)]

pub enum Token {
    Int(i64),
    Float(f64),

    Plus,
    Minus,
    Multiply,
    Divide,
}
