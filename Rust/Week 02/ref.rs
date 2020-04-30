fn main() {
	let s1 = String::from("hello");
	let len = calculate_length_2(&s1);
	println!("The length of '{}' is {}", s1, len);
	let (s3, len) = calculate_length_1(s1);

	let mut s = String::from("hello");
	change(&mut s);
	println!("The changed string is: {}", s);
}

/* 
   Change the string(mutable reference). 
   However, in a specific scope, only one
   mutable reference is allowed. Solution is
   we can create more scope by using {}
*/
fn change(some_string: &mut String){
	some_string.push_str(", world");
}

// reference -> borrowing
fn calculate_length_1(s: String) -> (String, usize){
	let length = s.len();
	return (s, length);
}

fn calculate_length_2(s: &String) -> usize{
	return s.len();
}
