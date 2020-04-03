#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Node
{
    struct Node *prev;
    char address[100];
    int requestCount;
    struct Node *next;
} NODE;

int T;
int L;
int count = 0;

NODE *isAddressExist(NODE *head, char address[]) //girdinin daha once alinip alinmadigi kontrol edilir.
{
    NODE *current = head;
    while (current->next != NULL && strcmp(current->address, address) != 0) //sona dogru giderek girdinin esit oldugu deger aranır.
    {                                                                       // esit olmadigi surece ilerlenir.
        current = current->next;
    }
    if (strcmp(current->address, address) == 0) //girdi mevcutsa adresini devamında kullanmak icin donduruyoruz.
        return current;
    return NULL;    // girdi yoksa onceden diger kosullara bakmak icin devam ediyoruz.
}

NODE *add(NODE *head, char address[])   
{
    if (head == NULL)   //listemiz bos ise basta yer acilir ve acilan yeni node'un icine alınan girdiler yerlestirilir.
    {
        NODE *node = (NODE *)malloc(sizeof(NODE));
        strcpy(node->address, address); //string islemi oldugu icin fonksiyon kullanarak esitleme yapiyoruz.
        node->requestCount = 1;
        node->next = NULL;  
        node->prev = NULL;
        return node;    //basta head null oldugundan ekledigimiz node'u yeni head olarak donduruyoruz.
    }
    NODE *searchNode = isAddressExist(head, address);   //aradigimiz girdi daha onceden alinmis mi diye kontrol etmeye gonderiyoruz.
    if (searchNode != NULL) // null'dan baska bir degerse daha once girilmis bir girdi almisiz demektir.
    {
        searchNode->requestCount++;     //girdinin listede bulunma sayisini arttiriyoruz.
        if (searchNode->requestCount == T + 1)  //kullanicinin belirledigi siniri gectiyse bulunma sayisi
        {
            searchNode->prev->next = searchNode->next;  //searchNode silinir.
            if (searchNode->next != NULL)  
                searchNode->next->prev = searchNode->prev; 
            searchNode->next = head;
            searchNode->prev = NULL;

            head->prev = searchNode;
            return searchNode;
        }
        return head;
    }
    else
    {
        NODE *node = (NODE *)malloc(sizeof(NODE));  //daha once girilmemis bir girdi alindiysa yer acilir
        strcpy(node->address, address);
        node->requestCount = 1;
        node->next = head;     //eklenen girdi basa eklenir. 
        head->prev = node;     //head ile yeni node baglanir.
        node->prev = NULL;     //head olacagi icin prev null olmali.
        count++;    //listemizde max eleman bulunma sayisini arttiririz.
        if (count == L)     // max sayiya ulastiysak silme islemi yapariz.
        {
            NODE *current = head;
            while (current->next != NULL)
            {
                current = current->next;    //listenin sonuna kadar geldigimizde
            }
            current->prev->next = NULL;     //sondaki node'u sileriz.
            free(current);  //sildigimiz node'u bellekten temizleriz.
            count--;    //listemizde eksilme oldugu icin sayacimizi 1 azaltiriz.
        }
        return node; //eklenen node'u head olarak dondururuz.
    }
}

void show(NODE *head)   //listemizi ekrana yazdirmak icin yazilan fonksiyondur.
{
    NODE *current = head;
    while (current->next != NULL)
    {
        printf("%s, %d <-> ", current->address, current->requestCount);
        current = current->next;
    }
    printf("%s, %d\n", current->address, current->requestCount);
}

NODE *readAddressesFromFile(NODE *head, char file[]) 
{
    char address[100];
    FILE *f = fopen(file, "r"); // dosyayı acip yapmak istedigimiz islemi belirtmemiz gerekir.
    feof(f);    //acilan text dosyasinda ilk satir T ve L icin ayrilmis olarak ayarlandi. 
    fscanf(f, "%d %d", &T, &L);     //dosyadan okuma islemi 1 kere yapilarak T ve L bilgileri alindi.
    while (!feof(f)) // dosyanin sonuna kadar okumak icin yazildi.
    {
        fscanf(f, "%s", address);   //dosyadan okuma islemi yapip alinan degerler "address"e atilir.
        head = add(head, address);  //kontrol edilen girdilerin islemleri yapilarak return edilen yeni head'ler bulunur.
        show(head);     //ekrana yazdirilir.
    }
    return head;    //yeni head devam edilmek icin dondurulur.
}
NODE *readAddressesFromConsole(NODE *head)
{
    int count = 0, i;
    printf("Adres sayisini giriniz: "); // gerekli bilgiler kullanicidan alinir.
    scanf("%d", &count);
    printf("T :");
    scanf("%d", &T);
    printf("L :");
    scanf("%d", &L);

    char **addresses = (char **)malloc(sizeof(char *) * count);

    for (i = 0; i < count; i++)
    {
        char *input = (char *)malloc(sizeof(char) * 100);
        scanf("%s", input);
        addresses[i] = input;   //alinan degerler tutulmak icin string dizisine aktarilir.
    }

    for (i = 0; i < count; i++)
    {
        head = add(head, addresses[i]);  //kontrol edilen girdilerin islemleri yapilarak return edilen yeni head'ler bulunur.
        show(head); //ekrana yazdirilir.
    }
    return head; //yeni head dındurulur.
}
int main()
{
    NODE *head = NULL; //listemizi bos olarak tanimliyoruz. en basta elimizde bir girdi olmadigi icin.
    int choice;
    printf("Bilgileri dosyadan okumak isterseniz 1'e konsoldan girmek isterseniz 2'ye basiniz :");
    while (choice != 1 && choice != 2) //dogru secimleri yapana kadar kullanici deger girer.
    {
        scanf("%d", &choice);
        if (choice == 1)
            head = readAddressesFromFile(head, "input.txt"); //dosyadan okuma icin
        else if (choice == 2)
            head = readAddressesFromConsole(head); //konsoldan giris icin
        else
            printf("Hatali giris yaptiniz, tekrar deneyiniz.");
    }

    int input;
    printf("\nOnbellegi silmek isterseniz 1 giriniz :");
    scanf("%d", &input);
    if (input == 1)  // kullanıcıya listeleme işleminden sonra cache’i temizlemek isteyip istemedigi sorulur.
    {
        NODE *current = head;
        while (current != NULL) 
        {
            NODE *tmp = current;
            current = current->next;
            free(tmp);
        }
        printf("Onbellek silindi.");
    }
    return 0;
}