#include<iostream>
#include<vector>
#include<map>
#include<fstream>

const std::string BANK_RECORDS = "bank_records.txt";

class Account
{
    private:
	std::string firstName;
	std::string lastName;
	double balance;
	int accountNumber;

    public:
    Account() {}
	Account(std::string firstName, std::string lastName, double balance, int accountNumber)
	{
	    this->firstName = firstName;
	    this->lastName = lastName;
	    this->balance = balance;
	    this->accountNumber = accountNumber;
	}

	void SetFirstName(std::string firstName)
	{
        this->firstName = firstName;
	}

	void SetLastName(std::string lastName)
	{
        this->lastName = lastName;
	}

	void SetBalance(double balance)
	{
        this->balance = balance;
	}

	void SetAccountNumber(int accountNumber)
	{
        this->accountNumber = accountNumber;
	}

	void Deposit(double amount)
	{
	    balance += amount;
	}

	void Withdrawal(double amount)
	{
        balance -= amount;
	}

	double GetBalance()
    {
        return balance;
	}

	std::string GetBalanceStr()
	{
        char buffer[20];
	    sprintf(buffer, "%s%.2f", "$", balance);
	    std::string balanceStr(buffer);
	    return balanceStr;
	}

	int GetAccountNumber()
	{
	    return accountNumber;
	}

	std::string GetAccountInfo()
	{
        std::string message = "First Name:     " + firstName + '\n';
	    message +=            "Last Name:      " + lastName + '\n';
	    message +=            "Account Number: " + std::to_string(accountNumber) + '\n';
	    message +=            "Balance:        " + GetBalanceStr() + "\n";
	    return message;
	}

	friend std::ofstream& operator<<(std::ofstream& ofs, Account* account);

	friend std::ifstream& operator>>(std::ifstream& ifs, Account* account);
};
std::ofstream& operator<<(std::ofstream& ofs, Account* account)
{
    ofs << account->firstName << std::endl;
    ofs << account->lastName << std::endl;
    ofs << account->accountNumber << std::endl;
    ofs << account->balance;
    return ofs;
}

std::ifstream& operator>>(std::ifstream& ifs, Account* account)
{
    std::string firstName;
    std::string lastName;
    int accountNumber;
    double balance;

    ifs >> firstName >> lastName >> accountNumber >> balance;
    account->SetFirstName(firstName);
    account->SetLastName(lastName);
    account->SetAccountNumber(accountNumber);
    account->SetBalance(balance);
    return ifs;
}

class Bank
{
    private:
	int accountCount;
	std::map<int, Account*> accounts;

	void OpenAccount()
	{
	    std::string firstName = "";
	    std::string lastName = "";
	    double balance = 0;
	    std::cout << "Enter First Name: ";
	    std::cin >> firstName;
	    std::cout << "Enter Last Name: ";
	    std::cin >> lastName;

        bool valid = false;
	    while (!valid)
        {
	        std::cout << "Enter Initial Balance: ";
	        std::cin >> balance;
            if (balance < 0)
            {
                std::cout << "Balance cannot be negative!\n" << std::endl;
            }
            else
            {
                valid = true;
            }
	    }

	    accountCount++;
	    Account* account = new Account(firstName, lastName, balance, accountCount);
	    accounts[accountCount] = account;
	    std::cout << "\nAccount is created!" << std::endl;
	    std::cout << account->GetAccountInfo() << std::endl;
	}

	Account* GetAccount()
	{
        std::cout << "Enter Account Number: ";
	    int accountNumber;
	    std::cin >> accountNumber;
	    Account* account = nullptr;

	    if (accounts.count(accountNumber) > 0)
	    {
            account = accounts.find(accountNumber)->second;
	    }
	    else
	    {
            std::cout << "Account not found.\n" << std::endl;
	    }
        return account;	    
	}

	void BalanceInquiry()
	{
        Account* account = GetAccount();
	    if (account != nullptr)
	    {
            std::cout << "Balance: " << account->GetBalanceStr() << "\n" << std::endl;
	    }
	}

	void Deposit()
	{
        Account* account = GetAccount();
	    if (account != nullptr)
	    {
            double amount;
            bool valid = false;
	        while (!valid)
            {
	            std::cout << "Enter Amount: ";
	            std::cin >> amount;
                if (amount < 0)
                {
                    std::cout << "Amount cannot be negative!\n" << std::endl;
                }
                else
                {
                    valid = true;
                }
	        }
	    	account->Deposit(amount);
	        std::cout << account->GetAccountInfo() << std::endl;        
	    }
	}

	void Withdrawal()
	{
        Account* account = GetAccount();
	    if (account != nullptr)
	    {
            double amount;
            bool valid = false;
	        while (!valid)
            {
	            std::cout << "Enter Amount: ";
	            std::cin >> amount;
		        if (amount < 0)
		        {
		            std::cout << "Amount cannot be negative!\n" << std::endl;
		        }
		        else
		        {
                    if (amount > account->GetBalance())
    			    {
                        std::cout << "Amount exceeds balance!" << std::endl;
        			}
        			else
        			{
                        valid = true;
        			}
    		    }
	        }
    		account->Withdrawal(amount);
	        std::cout << account->GetAccountInfo() << std::endl;        
	    }
	}

	void Close()
	{
        Account* account = GetAccount();
	    if (account != nullptr)
	    {
    		accounts.erase(account->GetAccountNumber());
            delete account;
	    	std::cout << "Account closed.\n" << std::endl;
	    }
	}

	void ShowAll()
	{
        std::map<int, Account*>::iterator itr;
	    for (itr = accounts.begin(); itr != accounts.end(); itr++)
	    {
            std::cout << itr->second->GetAccountInfo() << std::endl;
	    }
	}

    public:
	Bank()
	{
	    accountCount = 0;

	    std::ifstream inFile;
	    inFile.open(BANK_RECORDS);
        if (inFile.is_open())
        {
            Account* account;
	        while (!inFile.eof())
            {
                account = new Account();
                inFile >> account;
        	    accounts[account->GetAccountNumber()] = account;
            }            
    	    inFile.close();
        }
	}

	~Bank()
    {
        std::ofstream outFile(BANK_RECORDS, std::ios::trunc);
	    std::map<int, Account*>::iterator itr;
        for (itr = accounts.begin(); itr != accounts.end(); itr++)
        {
	        outFile << itr->second;
        }
	    outFile.close();
	}

	bool Menu()
	{
	    std::cout << "Select One Option Below\n" << std::endl;
	    std::cout << "1. Open an Account" << std::endl;
	    std::cout << "2. Balance Inquiry" << std::endl;
	    std::cout << "3. Deposit" << std::endl;
	    std::cout << "4. Withdrawal" << std::endl;
	    std::cout << "5. Close an Account" << std::endl;
	    std::cout << "6. Slow all Accounts" << std::endl;
	    std::cout << "7. Quit\n" << std::endl;

	    short selection;
	    std::cin >> selection;

	    switch (selection)
	    {
	        case 1:
		        OpenAccount();
		        break;
	        case 2:
		        BalanceInquiry();
		        break;
	        case 3:
		        Deposit();
		        break;
	        case 4:
		        Withdrawal();
		        break;
	        case 5:
		        Close();
		        break;
	        case 6:
		        ShowAll();
		        break;
	        case 7:
	            return false;
	    }
	    return true;
	}
};

int main()
{
    Bank bank;
    std::cout << "Welcome!\n" << std::endl;
    while ( bank.Menu() );
    std::cout << "Goodbye!\n" << std::endl;

    return 0;
}
