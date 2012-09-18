char emb[] = "abcdefghijklmnop";
int sum = 0xaabbccdd;

void test()
{
	int i;
	
	sum = 0xddccbbaa;
	for (i = 0; i < sizeof(emb)-1; i++)
	{
		emb[i] = 'z';
	}
	i = 0x12345678;
}

