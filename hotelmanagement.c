#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct items
{
    char item[20];
    float price;
    int qty;
};

struct orders
{
    char customer[50];
    char date[50];
    int numofitems;
    struct items itm[50];
};

// functions to generate bills
void generateBillHeader(char name[50], char date[30])
{
    printf("\n\n");
    printf("\t          B2A RESTAURANT");
    printf("\n\t        ----------------- ");
    printf("\nDate: %s", date);
    printf("\nInvoice To: %s", name);
    printf("\n");
    printf("------------------------------------\n");
    printf("Items\t\t");
    printf("Qty\t\t");
    printf("Total\t\t");
    printf("\n------------------------------------");
    printf("\n\n");
}

void generateBillBody(char item[30], int qty, float price)
{
    printf("%s\t\t", item);
    printf("%d\t\t", qty);
    printf("%.2f\t\t", qty * price);
    printf("\n");
}

void generateBillFooter(float total)
{
    printf("\n");
    float dis = 0.1 * total;
    float netTotal = total - dis;
    float cgst = 0.09 * netTotal, grandTotal = netTotal + 2 * cgst;
    printf("---------------------------------------------\n");
    printf("SubTotal\t\t\t%.2f", total);
    printf("\nDiscount @10%%\t\t\t%.2f", dis);
    printf("\n\t\t\t\t----------------");
    printf("\nNet Total\t\t\t%.2f", netTotal);
    printf("\nCGST @9%%\t\t\t%.2f", cgst);
    printf("\nSGST @9%%\t\t\t%.2f", cgst);
    printf("\n--------------------------------------------");
    printf("\nGrand Total\t\t\t%.2f", grandTotal);
    printf("\n--------------------------------------------\n");
}

int main()
{
    float total;

    int opt, n;
    struct orders ord;
    struct orders order;
    char saveBill = 'y', contFlag = 'y';
    char name[50];
    FILE *fp;

    // dashboard
    while (contFlag == 'y')
    {
        float total = 0;
        int invoiceFound = 0;
        printf("\t=================B2A RESTAURANT===============\n");
        printf("\nPlease select your preferred operation:\n");
        printf("1. Generate Invoice\n");
        printf("2. Show all Invoices\n");
        printf("3. Search Invoice\n");
        printf("4. Exit\n");

        printf("\nYour choice: ");
        scanf("%d", &opt);
        fflush(stdin);

        switch (opt)
        {
        case 1:
            system("clear");
            printf("\nEnter the name of the customer: ");
            fflush(stdin);
            fgets(ord.customer, 50, stdin);
            ord.customer[strlen(ord.customer) - 1] = '\0';
            printf("Enter the date: ");
            fgets(ord.date, 50, stdin);
            ord.date[strlen(ord.date) - 1] = '\0';
            printf("Enter the number of items: ");
            scanf("%d", &n);
            ord.numofitems = n;

            for (int i = 0; i < n; i++)
            {
                fflush(stdin);
                printf("\n\n");
                printf("Enter the item %d: ", i + 1);
                fgets(ord.itm[i].item, 20, stdin);
                ord.itm[i].item[strlen(ord.itm[i].item) - 1] = '\0';
                printf("Enter the quantity: ");
                scanf("%d", &ord.itm[i].qty);
                printf("Enter the unit price: ");
                scanf("%f", &ord.itm[i].price);
                total += ord.itm[i].qty * ord.itm[i].price;
            }
            generateBillHeader(ord.customer, ord.date);
            for (int i = 0; i < ord.numofitems; i++)
            {
                generateBillBody(ord.itm[i].item, ord.itm[i].qty, ord.itm[i].price);
            }
            generateBillFooter(total);

            printf("\nDo you want to save the invoice? [y/n]: ");
            scanf("%*c%c", &saveBill);

            if (saveBill == 'y')
            {
                fp = fopen("RestaurantBill.dat", "a+");
                fwrite(&ord, sizeof(struct orders), 1, fp);
                if (fwrite != 0)
                    printf("\nSuccessfully saved.");
                else
                    printf("\nError saving.");
                fclose(fp);
            }
            break;

        case 2:
            system("clear");
            fp = fopen("RestaurantBill.dat", "r");
            printf("\nYour previous invoices:\n");
            while (fread(&order, sizeof(struct orders), 1, fp))
            {
                float tot = 0;
                generateBillHeader(order.customer, order.date);
                for (int i = 0; i < order.numofitems; i++)
                {
                    generateBillBody(order.itm[i].item, order.itm[i].qty, order.itm[i].price);
                    tot += order.itm[i].qty * order.itm[i].price;
                }
                generateBillFooter(tot);
            }
            fclose(fp);
            break;

        case 3:
            printf("\nEnter the name of the customer: ");
            fflush(stdin);
            fgets(name, 50, stdin);
            name[strlen(name) - 1] = '\0';
            system("clear");
            fp = fopen("RestaurantBill.dat", "r");
            printf("\nInvoices of %s:\n", name);
            while (fread(&order, sizeof(struct orders), 1, fp))
            {
                float tot = 0;
                if (!strcmp(order.customer, name))
                {
                    generateBillHeader(order.customer, order.date);
                    for (int i = 0; i < order.numofitems; i++)
                    {
                        generateBillBody(order.itm[i].item, order.itm[i].qty, order.itm[i].price);
                        tot += order.itm[i].qty * order.itm[i].price;
                    }
                    generateBillFooter(tot);
                    invoiceFound = 1;
                }
            }
            if (!invoiceFound)
            {
                printf("Sorry, the invoice for %s does not exist.\n", name);
            }
            fclose(fp);
            break;

        case 4:
            printf("\n\t\tTHANK YOU :)\n\n");
            exit(0);
            break;

        default:
            printf("SORRY INVALID OPTION\n");
            break;
        }

        printf("\nDO YOU WANT TO PERFORM ANOTHER OPERATION? [y/n]: ");
        scanf("%*c%c", &contFlag);
    }

    printf("\n\n");
    return 0;
}
