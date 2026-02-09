fn main() {
    println!("Hello, world!");
}

fn main() {
    let mut file = File::create("example.txt").unwrap();
    file.write_all(b"Hello, World").unwrap();
}
