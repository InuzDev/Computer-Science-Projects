//! The source of the project is from https://doc.rust-lang.org/rust-by-example/hello/print.html

fn main() {
    // In general, the `{}` will be automatically replaced with any arguments.
    // These will be stringified.
    println!("{} days", 31);

    // Positional arguments can be used. Specifying an integer inside `{}`
    // determines which additional argument will be replaced. Arguments start
    // at 0 immediately after the format string.
    println!("{0}, this is {1}. {1}, this is {0}", "Alice", "bob");

    // As can named arguments.
    println!(
        "{subject} {verb} {object}",
        object = "The lazy dob",
        subject = "the quick brown fox",
        verb = "jumps over"
    );
}
