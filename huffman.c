#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define max 100

typedef struct node
{
    char letter;
    int frequency;
    struct node *left, *right, *next;
} NODE;
typedef struct que
{
    NODE *let[max]; //dugumdeki node'lardan olusan NODE* dizisi
    int top;        //queue ilk elamanin indexi
    int tail;       //queue son elemanin indexi
} QUE;

NODE *setNode(char letter, int frequency)
{
    NODE *node = (NODE *)malloc(sizeof(NODE)); //yeni node icin yer acilir
    node->letter = letter;                     //node un ici verilen bilgilerle doldurulur
    node->frequency = frequency;
    node->next = NULL;
    node->left = NULL;
    node->right = NULL;
    return node; //olusturulan node dondurulur
}
NODE *insertNode(NODE *head, NODE *newNode)
{
    NODE *node = newNode; //insert islemi oldugu icin node alinir
    if (head == NULL)     //bos bır dugum varsa basa eklenir
    {
        return node;
    }
    else //dugum bos degilse
    {
        NODE *current = head;
        while (current->next != NULL && current->next->frequency <= newNode->frequency)
        {                            //baktigim frekans yeni frekansımdan kucuk esit olana kadar devam edilir
            current = current->next; //index ilerletilir
        }
        if (current == head)
        {
            if (newNode->frequency < current->frequency) //yeni node head'den kucukse head degisir
            {
                node->next = current; //dugum ile current baglanir
                return node;          // yeni head dondurulur
            }
            NODE *temp = current->next; //yeni node ile head esitse
            current->next = node;       //yeni gelen head'in sagina eklenir
            node->next = temp;
            return head; //head degimez ve dondurulur
        }
        node->next = current->next; //araya ya da sona eklenecekse yeni node eklenir
        current->next = node;
        return head; //head degismez ve dondurulur
    }
}
NODE *huffman(NODE *head)
{
    NODE *left = head, *right = head->next;             // i ve i+1 node'lari alinir
    int freq = head->next->frequency + head->frequency; //yeni toplam frekans bulunur
    NODE *root = setNode(0, freq);                      //yeni frekansla letter olmadan yeni bir node olusturulur
    root->left = left;                                  //olusturulan node'un soluna ve sagina i ve i+1 node'lari yerlestirilir
    root->right = right;
    insertNode(head, root);   //olusan agac dugume eklenir
    NODE *temp = right->next; //head ilerletilir i+2 olur
    return temp;              //ilerleyen head dondurulur
}
NODE *textToFrequence(NODE *head, char text[])
{
    int i = 0;
    static int frequencies[27];        //26 harf ve bosluk oldugu icin 27'lik bir dizi acilir
    for (i = 0; i < strlen(text); i++) //alinan text'in boyutu kadar kontrol edilir
    {
        if (text[i] == ' ') //eger bosluk sayiliyorsa dizinin 26.elemani arttirilir
            frequencies[26]++;
        else
            frequencies[text[i] - 'a']++; //ascii tablosuna gore 'a' ile arasındaki fark alinarak frekans dizisindeki
                                          //yeri belirlenir
    }
    for (int i = 0; i < 26; i++) //frekanslar bulunduktan sonra
    {
        if (frequencies[i] != 0)                                       //text'te yer alan harfler dugume aktarilir
            head = insertNode(head, setNode('a' + i, frequencies[i])); //dizide bulunma indexi harfi verir
                                                                       //ornegin i=4 iken: setNode'un ilk parametresi char oldugu icin 'a'+4 yazdigimizda
                                                                       //'a'+4 un ascii karsiligini bulup e sonucunu aliriz
                                                                       //frekans dizisinin 4.elemani da e harfinin ne kadar tekrar ettigini gosterir
    }
    head = insertNode(head, setNode('/', frequencies[26])); //bosluk karakteri icin belirlenen / insert
                                                            //etmek icin gonderilir
    return head;                                            //insert islemlerinde head degisikligi olabilecegi icin head dondurulur
}
int isEmpty(QUE *que) // queue'nun bos olma durumu kontrol edilir
{
    return (que->tail == 0 && que->top == 0) || (que->tail - que->top == 1);
    //queue hicbir sey eklenmemisken tail ve top 0 olur
    //push+pop islemlerinden sonra tail ve que arasinda bir bosluk varsa
    //queue icinde eleman kalmamis demektir ve bostur
}
int push(QUE *que, NODE *node)
{
    que->let[que->tail] = node; //NODE* dizimize verilen node eklenir
    que->tail++;                //tail ilerletilir
}
NODE *pop(QUE *que)
{
    NODE *ret = que->let[que->top]; //queue'daki ilk eleman return edilir
    que->top++;                     //top'taki eleman donduruldugu icin top ilerletilir
    return ret;
}
void levelOrder(NODE *head) //line by line cikti icin
{
    NODE *node = NULL;                     //empty node olusturulur kontrol icin
    QUE *que = (QUE *)malloc(sizeof(QUE)); //empty queue olusturulur
    que->top = 0;                          //queue'nun ilk ve son indexi sifirlanir
    que->tail = 0;

    push(que, head); //head queue icine pushlanir
    push(que, NULL); //diger level'a gectigimiz icin queue'nun diger elemani null olur

    while (!isEmpty(que)) //queue bos olmadigi surece kontrol devam eder
    {
        NODE *node = pop(que); //queue'nun ilk elemani alinir
        if (node != NULL)      //null degilse bir state'tir
        {
            printf(" %d%c", node->frequency, node->letter); //yazdirilir
            if (node->left != NULL)                         //node'un null olmayan sol ve sag child'i queue icine push edilir
                push(que, node->left);
            if (node->right != NULL)
                push(que, node->right);
        }
        else if (node == NULL)
        {
            printf("\n");    //pop edilen deger null ise yeni satira gecmemiz gerekmektedir
            push(que, NULL); //ve o leveldaki node'larin sol-sag childlari eklenmis olup
                             //queue sonuna null eklenerek yeni level'a gectigimizi belli ederiz
        }
    }
}
int main()
{
    // !!!!!text icinde bosluk karakteri '/' olarak yazilacaktir !!!!!
    NODE *head = NULL;                               //bos bir dugum acilir
    char *text = (char *)malloc(sizeof(char) * 100); //alinacak string icin yer acilir
    printf("Yaziyi giriniz:");
    scanf("%[^\n]", text);              //kullanici enter tusuna basana kadarki stringi alir
    head = textToFrequence(head, text); //textteki harf sıklıgı bulunmak icin fonksiyona gonderilir
    while (head->next != NULL)          //huffman tree olusmasi icin dugumler fonksiyona gonderilir
    {
        head = huffman(head);
    }
    levelOrder(head); //Level-Order ile line by line huffman tree yazdirilir
    return 0;
}