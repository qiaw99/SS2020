fn my_println(message: &str) {
    println!("{:?}", message);
}

fn main() {
    let mut x = String::new();
    x.push_str("Hello ");
    x.push_str("World!");
    my_println(&x);              // &String -> &str              
}
