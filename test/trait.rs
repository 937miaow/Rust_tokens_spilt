trait Animal {
    fn name(&self) -> &str;
    fn talk(&self) {
        println!("{} says hello!", self.name());
    }
}

struct Dog {
    name: String,
}

impl Animal for Dog {
    fn name(&self) -> &str {
        &self.name
    }
    
    fn talk(&self) {
        println!("{} says woof!", self.name());
    }
}

struct Cat {
    name: String,
}

impl Animal for Cat {
    fn name(&self) -> &str {
        &self.name
    }
    
    fn talk(&self) {
        println!("{} says meow!", self.name());
    }
}

fn main() {
    let dog = Dog { name: String::from("Rex") };
    let cat = Cat { name: String::from("Whiskers") };
    
    dog.talk();
    cat.talk();
} 