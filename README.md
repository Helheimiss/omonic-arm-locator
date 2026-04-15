# Omonic Arm Locator

## Configs

### Server Configuration File

Uses standard Drogon configuration format:  
[config.json](https://drogonframework.github.io/drogon-docs/#/ENG/ENG-11-Configuration-File)



## Database Connection

Omonic uses ODBC driver for database connection.

### Connection String Example (MariaDB)
```
Driver={MariaDB ODBC 3.2 Driver};Server=localhost;Port=3306;Database=yourDB;UID=yourUSERNAME;PWD=yourPASSWORD;
```

### Setting Environment Variable

Set the `OMONIC_ARM_LOCATOR_CONNECTING_STRING` environment variable:

#### Linux (bash)

```bash
export OMONIC_ARM_LOCATOR_CONNECTING_STRING="Driver={MariaDB ODBC 3.2 Driver};Server=localhost;Port=3306;Database=yourDB;UID=yourUSERNAME;PWD=yourPASSWORD;"
```

#### Windows (CMD)

```cmd
set "OMONIC_ARM_LOCATOR_CONNECTING_STRING=Driver={MariaDB ODBC 3.2 Driver};Server=localhost;Port=3306;Database=yourDB;UID=yourUSERNAME;PWD=yourPASSWORD;"
```

#### Windows (PowerShell)

```powershell
$env:OMONIC_ARM_LOCATOR_CONNECTING_STRING = "Driver={MariaDB ODBC 3.2 Driver};Server=localhost;Port=3306;Database=yourDB;UID=yourUSERNAME;PWD=yourPASSWORD;"
```