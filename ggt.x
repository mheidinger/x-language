Groesster Gemeinsamer Teiler
=================================

-> a int
-> b int
c int
<- ergebnis int

=================================

// Euklidischer Algorithmus
if a < b {
	c = b
	b = a
	a = c
}

c = 1
while c > 0 {
	c = a % b
	b = a / b
	if c > 0 {
		a = b
		b = c
	}
}

ergebnis = c

