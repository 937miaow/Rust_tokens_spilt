fn main() {
    let x = 5;
    let y = 10;
    println!("x + y = {}", x + y);
    
    let mut vec = Vec::new();
    vec.push(1);
    vec.push(2);
    vec.push(3);
    
    for i in vec {
        println!("{}", i);
    }
} 