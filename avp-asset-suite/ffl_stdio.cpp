#include <stdio.h>
#include <string.h>

static void FFL_filedesc_init(char const* infoline, char const* path)
{
	// Normally class members:
	size_t dname_len;
	char* dir_name = 0;
	char* file_name = 0;

	if (infoline)
	{
		char dbuf[256];
		char fbuf[256];

		char* dbufP = dbuf;
		char* fbufP = fbuf;
		if (path)
		{
			strcpy(fbuf, path);
			fbufP += strlen(path);
		}

		int c1;
		int c2;

		do
		{
			c2 = c1 = *infoline++;
			if (';' == c1 || '\n' == c1)
				c1 = 0;
			*dbufP++ = (char)c1;
		} while (c1);

		if (c2)
		{
			do
			{
				c1 = *infoline++;
				if ('\n' == c1)
					c1 = 0;
				*fbufP++ = (char)c1;
			} while (c1);

			dname_len = strlen(dbuf);
			dir_name = new char[1 + dname_len];
			strcpy(dir_name, dbuf);
			file_name = new char[1 + strlen(fbuf)];
			strcpy(file_name, fbuf);

			printf("=> %s %s\n\n\n", dir_name, file_name);
		}
	}
}


int FFL_info_init(const char* infoFilePath, const char* fastfileDirectory)
{
	FILE* fp = fopen(infoFilePath, "r");
	if (!fp) return 0;

	// read data
	char buf[512];
	buf[0] = 0;

	fgets(buf, sizeof(buf), fp);

	do
	{
		// Only look at lines that start with a letter
		// mainly so as to avoid using blank lines
		
		if ((buf[0] >= 'a' && buf[0] <= 'z') ||
		   (buf[0] >= 'A' && buf[0] <= 'Z'))
		{
			printf("%s \n", buf);

			FFL_filedesc_init(buf, fastfileDirectory);
			// fdesclist.add_entry(FFileDesc(buf, fastfileDirectory));
		}
	}
	while (fgets(buf ,sizeof(buf), fp));

	fclose(fp);

	return 1;
}
