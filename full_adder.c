#include <stdio.h>
#include <stdlib.h>
typedef struct node //sayilarin basamaklarini tutmak icin
{                   //struct olusturuldu
    int n;
    struct node *next;
} NODE;
NODE *insertNode(NODE *head, int n) //eklenecek nodelar icin insert fonk yazildi
{
    if (head == NULL) //eger basta list bossa yeni ekledigimiz node
    {                 //head olarak dondurulur
        NODE *node = (NODE *)malloc(sizeof(NODE));
        node->n = n;
        node->next = NULL;
        return node;
    }
    NODE *current = head; //bos degilse list
    while (current->next != NULL) //listin son eklenen elemanina kadar gideriz
    {
        current = current->next;
    }
    NODE *newNode = (NODE *)malloc(sizeof(NODE)); //yeni bir node olusuturup
    newNode->n = n;
    newNode->next = NULL;
    current->next = newNode; //ilerlettigimiz currentin nextine ekleriz
    return head; //head degismez
}
NODE *reverseLinkedList(NODE *head) //nodeR icinde buldugumuz sayiyi(nodeI ve nodeJ 'daki sayilarin toplami tersten toplami)
{                                   //reverse edip duz gorebilmek icin fonksiyon yazildi
    NODE *reverse = NULL;
    while (head)
    {
        NODE *temp = head;
        head = head->next;
        temp->next = reverse;
        reverse = temp;
    }
    return reverse;
}

int main()
{
    int i, j;
    printf("Ilk sayiyi giriniz : "); //kullanicidan istenen bilgiler alinir
    scanf("%d", &i);
    printf("Ikinci sayiyi giriniz : ");
    scanf("%d", &j);
    NODE *nodeI = NULL; //listeler acilir, basta NULL olacak sekilde9
    NODE *nodeJ = NULL;
    NODE *nodeR = NULL;
    int tmpI = i, tmpJ = j;
    int ii = i, jj = j;
    while (tmpI > 0)
    {
        tmpI = i % 10; //ilk sayi icin basamaklarina ayirip liste eklenir
        nodeI = insertNode(nodeI, tmpI);
        i = i / 10;
    }
    while (tmpJ > 0)
    {
        tmpJ = j % 10; //ikinci sayi icin basamaklarina ayirip liste eklenir
        nodeJ = insertNode(nodeJ, tmpJ);
        j = j / 10;
    }
    int sum = 0;
    int n = 0;
    int control = 0;
    //bulunan nodeI ve nodeJ icindeki sayilar
    //son basamak basa gelecek sekilde listte bulunmaktadir
    //yazdigimiz fonksiyonla sondaki basamaklardan baskayip
    //eklentilerler toplamaya devam ederiz
    
    while (nodeI != NULL && nodeJ != NULL) 
    {
        sum = nodeI->n + nodeJ->n + control;
        if (sum >= 10) //elde durumu
            control = sum / 10;
        else
            control = 0;
        n = sum % 10;
        nodeR = insertNode(nodeR, n); 
        nodeI = nodeI->next;
        nodeJ = nodeJ->next;
    }
    if (nodeI == NULL)
    {
        while (nodeJ != NULL)
        {
            nodeR = insertNode(nodeR, nodeJ->n);
            nodeJ = nodeJ->next;
        }
    }
    if (nodeJ == NULL)
    {
        while (nodeI != NULL)
        {
            nodeR = insertNode(nodeR, nodeI->n);
            nodeI = nodeI->next;
        }
    }
    NODE *current = nodeR;
    while (current != NULL)
    {
        current = current->next;
    }
    nodeR = reverseLinkedList(nodeR);
    current = nodeR;
    printf("%d + %d = ", ii, jj);
    while (current != NULL)
    {
        printf("%d", current->n);
        current = current->next;
    }
    return 0;
}