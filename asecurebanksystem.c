#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Account {
    int accNo;
    char name[50];
    char password[20];
    float balance;
};

void createAccount() {
    struct Account acc;
    FILE *fp = fopen("accounts.dat", "ab");

    printf("\n=== Create New Account ===\n");
    printf("Enter account number: ");
    scanf("%d", &acc.accNo);
    printf("Enter your name: ");
    scanf(" %[^\n]", acc.name);
    printf("Create a password: ");
    scanf("%s", acc.password);
    acc.balance = 0;

    fwrite(&acc, sizeof(acc), 1, fp);
    fclose(fp);

    printf("Account created successfully!\n");
}

int login(struct Account *acc) {
    FILE *fp = fopen("accounts.dat", "rb");
    int found = 0;
    int inputAcc;
    char inputPass[20];

    printf("\n=== Login ===\n");
    printf("Enter account number: ");
    scanf("%d", &inputAcc);
    printf("Enter password: ");
    scanf("%s", inputPass);

    while (fread(acc, sizeof(*acc), 1, fp)) {
        if (acc->accNo == inputAcc && strcmp(acc->password, inputPass) == 0) {
            found = 1;
            break;
        }
    }

    fclose(fp);
    if (!found) printf("Login failed! Incorrect account number or password.\n");
    return found;
}

void saveAccount(struct Account acc) {
    FILE *fp = fopen("accounts.dat", "rb+");
    struct Account temp;
    while (fread(&temp, sizeof(temp), 1, fp)) {
        if (temp.accNo == acc.accNo) {
            fseek(fp, -sizeof(temp), SEEK_CUR);
            fwrite(&acc, sizeof(acc), 1, fp);
            break;
        }
    }
    fclose(fp);
}

void userMenu(struct Account acc) {
    int choice;
    float amt;

    do {
        printf("\n=== Bank Menu ===\n");
        printf("1. Check Balance\n");
        printf("2. Deposit\n");
        printf("3. Withdraw\n");
        printf("4. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Current Balance: Rs. %.2f\n", acc.balance);
                break;
            case 2:
                printf("Enter amount to deposit: ");
                scanf("%f", &amt);
                acc.balance += amt;
                saveAccount(acc);
                printf("Amount deposited successfully!\n");
                break;
            case 3:
                printf("Enter amount to withdraw: ");
                scanf("%f", &amt);
                if (amt > acc.balance)
                    printf("Insufficient balance!\n");
                else {
                    acc.balance -= amt;
                    saveAccount(acc);
                    printf("Withdrawal successful!\n");
                }
                break;
            case 4:
                printf("Logged out successfully.\n");
                break;
            default:
                printf("Invalid option!\n");
        }
    } while (choice != 4);
}

int main() {
    int choice;
    struct Account acc;

    do {
        printf("\n===== Secure Bank System =====\n");
        printf("1. Create Account\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                createAccount();
                break;
            case 2:
                if (login(&acc)) {
                    printf("Login successful! Welcome, %s\n", acc.name);
                    userMenu(acc);
                }
                break;
            case 3:
                printf("Thank you for using our bank system.\n");
                exit(0);
            default:
                printf("Invalid choice!\n");
        }
    } while (1);

    return 0;
}
