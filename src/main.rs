use std::io::{self, Write};

use lexer::tokenizer::tokenize;
use parser::parser::parse;

mod evaluator;
mod lexer;
mod parser;

fn main() {
    loop {
        let mut input = String::new();

        print!(">>> ");
        io::stdout().flush().expect("OOps");
        io::stdin().read_line(&mut input).unwrap();
        if input == "exit\n".to_string() {
            println!("Bye bye!");
            break;
        }

        let tokens = tokenize(&input);

        let ast = parse(tokens.unwrap()).unwrap();

        let val = evaluator::evaluate(&ast);
        if val.is_int() {
            println!("{}", val.as_int());
        } else {
            println!("{}", val.as_float());
        }
    }
}
