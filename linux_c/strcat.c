#include <stdio.h>
#include <string.h>

char* my_strcat(char* pd, char* ps) {
	char* po = pd;

	while (*pd != '\0') {
		pd++;
	}
	while (*ps != '\0') {
		*pd = *ps;
		pd++;
		ps++;
	}
	*pd = '\0';
	return po;
}

int main(void)
{
	char fruit[80] = "straw";
	my_strcat(fruit, "berry");
	printf("연결된 문자열 : %s\n", fruit);
}