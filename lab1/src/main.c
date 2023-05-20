int main(int argc, char *argv[]) 
{
    int i;

    printf("A complete list of command line arguments:\n");
    for (i = 0; i < argc; i++) {
        printf("argv[%d]=%s\n", i, argv[i]);
    }
    return 0;
}