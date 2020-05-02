fn print_collection(x: &[u32]) {
    println!("{:#?}", x);
}

fn main() {
    let mut x = Vec::new();
    x.push(17);
    x.push(22);
    x.push(5);
    print_collection(&x);        // &Vec<u32> -> &[u32]
}
