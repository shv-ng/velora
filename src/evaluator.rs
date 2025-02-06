use crate::parser::{self, expression::BinaryOp};

pub fn evaluate(ast: &BinaryOp) -> parser::expression::Number {
    ast.evaluate()
}
