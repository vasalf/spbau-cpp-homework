#include <phonebook.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

int main(int argc, char **argv) {
    srand(time(NULL));

    assert(argc == 3);
    phonebook_t inbook;

    load_phonebook_xml(argv[1], &inbook);
    print_phonebook(&inbook);
    
    phonebook_t outbook;
    
    gen_phonebook(&outbook, rand() % 20);
    save_phonebook_xml(argv[2], &outbook);

    clear_phonebook(&inbook);
    clear_phonebook(&outbook);

    return 0;
}

