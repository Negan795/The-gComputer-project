


use std::fs::File;
use std::io::{self, BufRead};
use std::path::Path;



fn main() {
    

    //variables
    let mut out = 0u32;
    let mut work = String::new();
    let mut line_num = 1u32;
    let mut cut_num: usize;

    //vectors
    let mut condition: Vec<u8> = (0..0).collect();
    let mut instruction: Vec<u8> = (0..0).collect();
    let mut argument1: Vec<u32> = (0..0).collect();
    let mut argument2: Vec<u32> = (0..0).collect();
    let mut argument3: Vec<u32> = (0..0).collect();



    if let Ok(lines) = read_lines("readfile") {

        for line in lines.map_while(Result::ok) {
            
            line_num += 1;

            work = parse_line(line);

            if &work != ""{

                //condition
                if &work[0..2] == "if"{
                    
                    work = cut(work, 2);

                    match &work[0..1]{
                        ">" => condition.push(1),
                        "=" => condition.push(2),
                        "<" => condition.push(3),
                        "l" => condition.push(4),
                        "s" => condition.push(5),
                        "m" => condition.push(6),
                        _   => condition.push(255),  //unrecognised conditionition
                    }

                    work = cut(work, 1);


                }else {
                    condition.push(0);
                }

                
                //instruction
                if work.len() > 2{
                    match &work[0..3]{
                        "nop" => instruction.push(0),     
                        "trs" => instruction.push(1),
                        "mov" => instruction.push(2),
                        "lim" => instruction.push(3),     
                        "add" => instruction.push(4),
                        "sub" => instruction.push(5),
                        "ad8" => instruction.push(6),
                        "a16" => instruction.push(7),
                        "sb8" => instruction.push(8),
                        "s16" => instruction.push(9),
                        "inc" => instruction.push(10),
                        "dec" => instruction.push(11),     
                        "and" => instruction.push(12),
                        "orr" => instruction.push(13),
                        "xor" => instruction.push(14),
                        "shl" => instruction.push(15),
                        "shr" => instruction.push(16),
                        "rol" => instruction.push(17),
                        "ror" => instruction.push(18),
                        "psh" => instruction.push(19),
                        "pop" => instruction.push(20),
                        "bst" => instruction.push(21),
                        "bcl" => instruction.push(22),
                        "btg" => instruction.push(23),
                        "jmp" => instruction.push(24),  
                        "int" => instruction.push(25),
                        _     => instruction.push(254), //unrecognised instruction
                    }

                    work = cut(work, 3);

                }else{
                    instruction.push(255);  //incomplete instruction
                }


                //arguments
                if work.len() > 1{

                    //make argument loop

                    cut_num = 0;

                    if &work[0..1] == "r" || &work[0..1] == "R"{
                        if work[1..2].parse::<u32>().is_ok() && !(work[1..3].parse::<u32>().is_ok()){
                            argument1.push(work[1..2].parse::<u32>().unwrap());    
                            cut_num = 1;
                        }
                        else if work[1..3].parse::<u32>().is_ok(){
                            argument1.push(work[1..3].parse::<u32>().unwrap());
                            cut_num = 2;
                        }
                        else if work[1..4].parse::<u32>().is_ok(){
                            argument1.push(work[1..4].parse::<u32>().unwrap());
                            cut_num = 3;
                        }
                        else{
                            argument1.push(255);
                        }
                        
                    }

                    if &work[0..1] == "a" || &work[0..1] == "A"{
                        if work[1..2].parse::<u32>().is_ok() && !(work[1..3].parse::<u32>().is_ok()){
                            argument1.push(work[1..2].parse::<u32>().unwrap()<<16);    
                            cut_num = 1;
                        }
                        else if work[1..3].parse::<u32>().is_ok() && !(work[1..4].parse::<u32>().is_ok()){
                            argument1.push(work[1..3].parse::<u32>().unwrap()<<16);
                            cut_num = 2;
                        }
                        else if work[1..4].parse::<u32>().is_ok(){
                            argument1.push(work[1..4].parse::<u32>().unwrap()<<16);
                            cut_num = 3;
                        }
                        else{
                            argument1.push(255);
                        }
                        
                    }

                    work = cut(work, 1+cut_num);
                }

            }

        }

    }

    print!("\nCONDITION:  ");
    for x in condition.iter(){
        print!("{}, ", x);
    }
    print!("\nINSTRUCION: ");
    for x in instruction.iter(){
        print!("{}, ", x);
    }
    print!("\nARGUMENT1:  ");
    for x in argument1.iter(){
        print!("{}, ", x);
    }

}


/*
let mut input = Strng::new();
input.clear();
io::stdin().read_line(&mut input).expect("    READ ERROR!");
input = input.trim().to_string();
*/






fn parse_line(str: String) -> String{

    let mut strr = String::new();

    for i in 0..str.len(){

        match &str[i..i+1] {
            " " | "\t" => continue,
            "#" | "\n" => break,
            _ => strr.push_str(&str[i..i+1]),
        }

    }

    return strr;

}


fn cut(str: String, amount: usize) -> String{

    let mut strr = String::new();

    for i in amount..str.len(){

        strr.push_str(&str[i..i+1]);

    }

    return strr; 

}




//not mine
fn read_lines<P>(filename: P) -> io::Result<io::Lines<io::BufReader<File>>>
    where P: AsRef<Path>, {
    let file = File::open(filename)?;
    Ok(io::BufReader::new(file).lines())
}