#include <sys/types.h>

typedef struct B5_hash
{
	int	idx;	// index
	struct B5_hash	*next;
} B5_hash_t;

#define HASH_TABLE_SIZE 40000

int hash(u_char *);
void B5toPhonetic_Init();
u_char *B5toPhone(u_char *data);
void B5toPhonetic_Final();
int B5toPhonetic(u_char *, u_char *);
int _B5toPhonetic(u_char *, u_char *);
