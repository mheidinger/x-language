Factorial Calculation
==================================

-> input_num int
<- result int
iterator int

==================================

result = 1
if input_num != 0 {
	for iterator = 2; iterator < input_num + 1; iterator = iterator + 1 {
		result = result * iterator
	}
}