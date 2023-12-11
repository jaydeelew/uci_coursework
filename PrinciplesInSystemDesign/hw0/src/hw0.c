int main(int argc, char *argv[])
{
	for (int i = argc - 1; i >= 0; --i)
	{
		printArg(argv[i], i);
	}

	for (int i = 1; i < argc; ++i)
	{
		sumArg(argv[i]);
	}
	printf("Sum: %d\n", sumArg("0"));
	return 0;
}

void printArg(char * arg_str, int pos)
{
	printf("Argument[%d]: %s\n", pos, arg_str);	
}

int sumArg(char * arg_str)
{
	static int sum;
	sum += atoi(arg_str);
	return sum;
}