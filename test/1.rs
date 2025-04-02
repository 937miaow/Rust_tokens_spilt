fn main() {
    let max_iterations = 1_000_000; // 设置迭代次数
    let mut denominator = 1; // 分母初始化为1
    let mut pi_approx = 0.0; // 初始化pi的近似值
    let mut sign = 1.0; // 初始化符号为正
 
    for _ in 0..max_iterations {
        pi_approx += sign * 4.0 / denominator;
        denominator += 2; // 分母递增2
        sign = -sign; // 符号变号
    }
 
    let pi = pi_approx * 2.0; // 最终的pi值是近似值乘以2
    println!("计算得到的pi值是: {}", pi);

    let hex_number = 0x1A3F_CDEF;
    let number1 =98_222;
}