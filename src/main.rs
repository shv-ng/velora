use lexer::tokenizer::tokenize;
use parser::parser::parse;

mod lexer;
mod parser;

fn main() {
    let input = "2+2.3-2";
    let tokens = tokenize(input);
    println!("{:?}", input);
    println!("{:?}", tokens);

    let p = parse(tokens.unwrap());
    println!("{p:#?}");
    let ast = p.unwrap();
    println!("{:?}", ast.evaluate());
}
