#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h> // 用于密码输入
#include <ctype.h> // 用于字符处理

// 定义时间转换结构体
typedef struct {
    float hours;
    float days;
} TimeConverter;

// 定义休假记录结构体
typedef struct {
    char employeeId[20];
    char name[50];
    float sickLeaveDays;  // 病假天数
    float maxSickLeaveDays;  // 最大病假天数
    float paidLeaveDays;   // 带薪休假天数
    float maxPaidLeaveDays;   // 最大带薪休假天数
} LeaveRecord;

// 定义员工基类结构体
typedef struct {
    char id[20];   // 员工编号
    char name[50];   // 姓名
    char gender[10];  // 性别
    char department[50];    // 部门
    int level;    // 级别
    float salary;       // 基本工资
    LeaveRecord leave;    // 休假记录
} Employee;   

// 定义经理结构体
typedef struct {
    Employee base;   // 基类员工信息
    float bonus;   // 奖金
} Manager;

// 定义技术人员结构体
typedef struct {
    Employee base;   // 基类员工信息
    float overtimePay;   // 加班费
} Technician;

// 定义销售人员结构体
typedef struct {
    Employee base;    // 基类员工信息
    float sales;   // 销售额
    float commission;       // 提成比例
} SalesPerson;

// 定义销售经理结构体
typedef struct {
    Employee base;     // 基类员工信息
    float bonus;    // 奖金
    float sales;    // 销售额
    float commission;      // 提成比例
} SalesManager;

// 用户账户结构体
typedef struct {
    char username[50];
    char password[50];
    int isAdmin;
} UserAccount;

// 全局变量
Employee* employees[1000];   // 员工数组
UserAccount users[100];     // 用户数组
int employeeCount = 0;     // 员工数量
int userCount = 0;    // 用户数量
UserAccount currentUser;     // 当前用户

// 函数声明
void initSystem();    // 初始化系统
void getPassword(char *password);    // 密码输入函数
int login();    // 登录函数
void adminMenu();    // 管理员菜单
void userMenu();    // 普通用户菜单
void addUser();    // 添加用户
void resetPassword();    // 重置用户密码
void changePassword();    // 修改密码
void addEmployee();       // 添加员工
void searchEmployee();     // 查询员工
void displayEmployee(Employee *emp);    // 显示员工信息
void displayAllEmployees();      // 显示所有员工
void editEmployee();      // 编辑员工
void deleteEmployee();     // 删除员工
void addLeaveRecord();      // 添加休假记录
TimeConverter convertHoursToDays(float hours);     // 小时转换为天数
void displayStatistics();     // 显示统计信息
void saveData();     // 保存数据
void loadData();     // 加载数据
void clearInputBuffer();     // 清空输入缓冲区
void mainMenu();     // 显示主菜单

// 主函数
int main() {
    mainMenu(); // 显示主菜单
    initSystem(); // 初始化系统

    while (1) {
        system("cls"); // 清屏
        int choice;
        scanf("%d", &choice); // 读取用户选择
        clearInputBuffer();

        switch (choice) {
            case 1: // 登录
            system("cls"); // 清屏
                if (login()) {
                    printf("\n登录成功! 欢迎, %s\n", currentUser.username);
                    if (currentUser.isAdmin) {
                        adminMenu(); // 管理员菜单
                    } else {
                        userMenu(); // 普通用户菜单
                    }
                } else {
                    printf("用户名或密码错误! 请重试.\n");
                }
                break;
            case 0: // 退出系统
                printf("退出系统...\n");
                return 0;
            default:
                printf("无效选择，请重试!\n");
        }
    }

    return 0;
}

//加载主页面
void mainMenu()
{
    printf("\n=================== 员工考勤管理系统 ================\n");
    printf("====1.登录               2.添加用户                ====\n");
    printf("====3.重置密码           4.添加员工                ====\n");
    printf("====5.查询员工           6.显示所有员工            ====\n");
    printf("====7.编辑员工           8.删除员工                ====\n");
    printf("====9.统计信息           10.添加休假记录           ====\n");
    printf("====11.保存数据          12.加载数据               ====\n");
    printf("====0.退出系统           13.修改密码               ====\n");
    printf("====================================================\n");
    printf("请选择操作: ");
}

// 初始化系统
void initSystem() {
    // 初始化管理员账户
    strcpy(users[0].username, "admin");    
    strcpy(users[0].password, "123456");
    users[0].isAdmin = 1; 
    userCount = 1;
    
    // 尝试加载已有数据
    loadData();
}

// 密码输入函数(显示*)
void getPassword(char *password) {
    int i = 0;
    char ch;
    while (1) {
        ch = getch();
        if (ch == '\r') {
            password[i] = '\0';
            break;
        } else if (ch == '\b') {
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else {
            password[i] = ch;
            i++;
            printf("*");
        }
    }
}

// 登录函数
int login() {
    char username[50];
    char password[50];
    
    printf("\n=== 员工考勤管理系统 ===\n");
    printf("用户名: ");
    scanf("%s", username);
    clearInputBuffer();
    
    printf("密码: ");
    getPassword(password);
    printf("\n");
    
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0 && 
            strcmp(users[i].password, password) == 0) {
            currentUser = users[i];
            return 1;
        }
    }
    return 0;
}

// 管理员菜单
void adminMenu() {
    int choice;
    while (1) {
        printf("\n=== 管理员菜单 ===\n");
        printf("1. 添加用户\n2. 重置用户密码\n");
        printf("3. 添加员工\n4. 查询员工\n5. 显示所有员工\n");
        printf("6. 编辑员工\n7. 删除员工\n8. 统计信息\n");
        printf("9. 添加休假记录\n10. 保存数据\n11. 加载数据\n");
        printf("0. 退出\n");
        printf("选择: ");
        
        scanf("%d", &choice);
        clearInputBuffer();
        
        switch (choice) {
            case 1: addUser(); break;
            case 2: resetPassword(); break;
            case 3: addEmployee(); break;
            case 4: searchEmployee(); break;
            case 5: displayAllEmployees(); break;
            case 6: editEmployee(); break;
            case 7: deleteEmployee(); break;
            case 8: displayStatistics(); break;
            case 9: addLeaveRecord(); break;
            case 10: saveData(); break;
            case 11: loadData(); break;
            case 0: return;
            default: printf("无效选择!\n");
        }
    }
}

// 普通用户菜单
void userMenu() {
    int choice;
    while (1) {
        printf("\n=== 用户菜单 ===\n");
        printf("1. 修改密码\n");
        printf("2. 查询员工\n3. 显示所有员工\n");
        printf("0. 退出\n");
        printf("选择: ");
        
        scanf("%d", &choice);
        clearInputBuffer();
        
        switch (choice) {
            case 1: changePassword(); break;
            case 2: searchEmployee(); break;
            case 3: displayAllEmployees(); break;
            case 0: return;
            default: printf("无效选择!\n");
        }
    }
}

// 添加用户
void addUser() {
    if (!currentUser.isAdmin) {
        printf("权限不足!\n");
        return;
    }
    
    if (userCount >= 100) {
        printf("用户数量已达上限!\n");
        return;
    }
    
    UserAccount newUser;
    printf("输入用户名: ");
    scanf("%s", newUser.username);
    clearInputBuffer();
    
    // 检查用户名是否已存在
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, newUser.username) == 0) {
            printf("用户名已存在!\n");
            return;
        }
    }
    
    printf("输入密码: ");
    scanf("%s", newUser.password);
    clearInputBuffer();
    
    printf("设置用户类型 (1-管理员, 0-普通用户): ");
    scanf("%d", &newUser.isAdmin);
    clearInputBuffer();
    
    users[userCount++] = newUser;
    printf("用户添加成功!\n");
}

// 重置用户密码
void resetPassword() {
    if (!currentUser.isAdmin) {
        printf("权限不足!\n");
        return;
    }
    
    char username[50];
    printf("输入要重置密码的用户名: ");
    scanf("%s", username);
    clearInputBuffer();
    
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0) {
            strcpy(users[i].password, "1");
            printf("用户 %s 的密码已重置为 1\n", username);
            return;
        }
    }
    printf("未找到用户 %s\n", username);
}

// 修改密码
void changePassword() {
    char oldPassword[50], newPassword[50], confirmPassword[50];
    
    printf("输入旧密码: ");
    getPassword(oldPassword);
    printf("\n");
    
    if (strcmp(oldPassword, currentUser.password) != 0) {
        printf("旧密码不正确!\n");
        return;
    }
    
    printf("输入新密码: ");
    getPassword(newPassword);
    printf("\n");
    
    printf("确认新密码: ");
    getPassword(confirmPassword);
    printf("\n");
    
    if (strcmp(newPassword, confirmPassword) != 0) {
        printf("两次输入的密码不一致!\n");
        return;
    }
    
    // 更新当前用户和全局用户数组中的密码
    strcpy(currentUser.password, newPassword);
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, currentUser.username) == 0) {
            strcpy(users[i].password, newPassword);
            break;
        }
    }
    
    printf("密码修改成功!\n");
}

// 添加员工
void addEmployee() {
    if (employeeCount >= 1000) {
        printf("员工数量已达上限!\n");
        return;
    }
    
    int type;
    printf("选择员工类型:\n");
    printf("1. 经理\n2. 技术人员\n3. 销售人员\n4. 销售经理\n");
    printf("选择: ");
    scanf("%d", &type);
    clearInputBuffer();
    
    if (type < 1 || type > 4) {
        printf("无效的员工类型!\n");
        return;
    }
    
    // 检查编号是否唯一
    char id[20];
    printf("输入员工编号: ");
    scanf("%s", id);
    clearInputBuffer();
    
    for (int i = 0; i < employeeCount; i++) {
        if (strcmp(employees[i]->id, id) == 0) {
            printf("错误: 员工编号已存在!\n");
            return;
        }
    }
    
    // 根据类型创建不同员工
    switch (type) {
        case 1: { // 经理
            Manager *mgr = (Manager*)malloc(sizeof(Manager));
            printf("输入姓名: "); scanf("%s", mgr->base.name);
            printf("输入性别: "); scanf("%s", mgr->base.gender);
            printf("输入部门: "); scanf("%s", mgr->base.department);
            printf("输入级别: "); scanf("%d", &mgr->base.level);
            printf("输入基本工资: "); scanf("%f", &mgr->base.salary);
            printf("输入奖金: "); scanf("%f", &mgr->bonus);
            strcpy(mgr->base.id, id);
            
            // 初始化休假记录
            strcpy(mgr->base.leave.employeeId, id);
            strcpy(mgr->base.leave.name, mgr->base.name);
            mgr->base.leave.sickLeaveDays = 0;
            mgr->base.leave.maxSickLeaveDays = 2; // 16小时=2天
            mgr->base.leave.paidLeaveDays = 0;
            mgr->base.leave.maxPaidLeaveDays = 3; // 24小时=3天
            
            employees[employeeCount++] = (Employee*)mgr;
            break;
        }
        case 2: { // 技术人员
            Technician *tech = (Technician*)malloc(sizeof(Technician));
            printf("输入姓名: "); scanf("%s", tech->base.name);
            printf("输入性别: "); scanf("%s", tech->base.gender);
            printf("输入部门: "); scanf("%s", tech->base.department);
            printf("输入级别: "); scanf("%d", &tech->base.level);
            printf("输入基本工资: "); scanf("%f", &tech->base.salary);
            printf("输入加班费: "); scanf("%f", &tech->overtimePay);
            strcpy(tech->base.id, id);
            
            // 初始化休假记录
            strcpy(tech->base.leave.employeeId, id);
            strcpy(tech->base.leave.name, tech->base.name);
            tech->base.leave.sickLeaveDays = 0;
            tech->base.leave.maxSickLeaveDays = 2;
            tech->base.leave.paidLeaveDays = 0;
            tech->base.leave.maxPaidLeaveDays = 3;
            
            employees[employeeCount++] = (Employee*)tech;
            break;
        }
        case 3: { // 销售人员
            SalesPerson *sales = (SalesPerson*)malloc(sizeof(SalesPerson));
            printf("输入姓名: "); scanf("%s", sales->base.name);
            printf("输入性别: "); scanf("%s", sales->base.gender);
            printf("输入部门: "); scanf("%s", sales->base.department);
            printf("输入级别: "); scanf("%d", &sales->base.level);
            printf("输入基本工资: "); scanf("%f", &sales->base.salary);
            printf("输入销售额: "); scanf("%f", &sales->sales);
            printf("输入提成比例(0-1): "); scanf("%f", &sales->commission);
            strcpy(sales->base.id, id);
            
            // 初始化休假记录
            strcpy(sales->base.leave.employeeId, id);
            strcpy(sales->base.leave.name, sales->base.name);
            sales->base.leave.sickLeaveDays = 0;
            sales->base.leave.maxSickLeaveDays = 2;
            sales->base.leave.paidLeaveDays = 0;
            sales->base.leave.maxPaidLeaveDays = 3;
            
            employees[employeeCount++] = (Employee*)sales;
            break;
        }
        case 4: { // 销售经理
            SalesManager *salesMgr = (SalesManager*)malloc(sizeof(SalesManager));
            printf("输入姓名: "); scanf("%s", salesMgr->base.name);
            printf("输入性别: "); scanf("%s", salesMgr->base.gender);
            printf("输入部门: "); scanf("%s", salesMgr->base.department);
            printf("输入级别: "); scanf("%d", &salesMgr->base.level);
            printf("输入基本工资: "); scanf("%f", &salesMgr->base.salary);
            printf("输入奖金: "); scanf("%f", &salesMgr->bonus);
            printf("输入销售额: "); scanf("%f", &salesMgr->sales);
            printf("输入提成比例(0-1): "); scanf("%f", &salesMgr->commission);
            strcpy(salesMgr->base.id, id);
            
            // 初始化休假记录
            strcpy(salesMgr->base.leave.employeeId, id);
            strcpy(salesMgr->base.leave.name, salesMgr->base.name);
            salesMgr->base.leave.sickLeaveDays = 0;
            salesMgr->base.leave.maxSickLeaveDays = 2;
            salesMgr->base.leave.paidLeaveDays = 0;
            salesMgr->base.leave.maxPaidLeaveDays = 3;
            
            employees[employeeCount++] = (Employee*)salesMgr;
            break;
        }
    }
    printf("员工添加成功!\n");
}

// 查询员工
void searchEmployee() {
    if (employeeCount == 0) {
        printf("当前没有员工记录!\n");
        return;
    }
    
    int choice;
    printf("查询方式:\n");
    printf("1. 按编号\n2. 按姓名\n");
    printf("选择: ");
    scanf("%d", &choice);
    clearInputBuffer();
    
    if (choice == 1) {
        char id[20];
        printf("输入员工编号: ");
        scanf("%s", id);
        clearInputBuffer();
        
        for (int i = 0; i < employeeCount; i++) {
            if (strcmp(employees[i]->id, id) == 0) {
                displayEmployee(employees[i]);
                return;
            }
        }
        printf("未找到编号为 %s 的员工\n", id);
    } else if (choice == 2) {
        char name[50];
        printf("输入员工姓名: ");
        scanf("%s", name);
        clearInputBuffer();
        
        int found = 0;
        for (int i = 0; i < employeeCount; i++) {
            if (strcmp(employees[i]->name, name) == 0) {
                displayEmployee(employees[i]);
                found = 1;
            }
        }
        if (!found) printf("未找到姓名为 %s 的员工\n", name);
    } else {
        printf("无效的选择!\n");
    }
}

// 显示员工信息
void displayEmployee(Employee *emp) {
    printf("\n员工编号: %s\n", emp->id);
    printf("姓名: %s\n", emp->name);
    printf("性别: %s\n", emp->gender);
    printf("部门: %s\n", emp->department);
    printf("级别: %d\n", emp->level);
    printf("基本工资: %.2f\n", emp->salary);
    
    // 显示特定类型员工的额外信息
    if (strcmp(emp->department, "经理") == 0) {
        Manager *mgr = (Manager*)emp;
        printf("奖金: %.2f\n", mgr->bonus);
        printf("总工资: %.2f\n", emp->salary + mgr->bonus);
    } else if (strcmp(emp->department, "技术") == 0) {
        Technician *tech = (Technician*)emp;
        printf("加班费: %.2f\n", tech->overtimePay);
        printf("总工资: %.2f\n", emp->salary + tech->overtimePay);
    } else if (strcmp(emp->department, "销售") == 0) {
        SalesPerson *sales = (SalesPerson*)emp;
        printf("销售额: %.2f\n", sales->sales);
        printf("提成: %.2f\n", sales->sales * sales->commission);
        printf("总工资: %.2f\n", emp->salary + sales->sales * sales->commission);
    } else if (strcmp(emp->department, "销售管理") == 0) {
        SalesManager *salesMgr = (SalesManager*)emp;
        printf("奖金: %.2f\n", salesMgr->bonus);
        printf("销售额: %.2f\n", salesMgr->sales);
        printf("提成: %.2f\n", salesMgr->sales * salesMgr->commission);
        printf("总工资: %.2f\n", emp->salary + salesMgr->bonus + 
               salesMgr->sales * salesMgr->commission);
    }
    
    // 显示休假信息
    printf("\n休假信息:\n");
    printf("病假天数: %.1f/%.1f\n", emp->leave.sickLeaveDays, emp->leave.maxSickLeaveDays);
    printf("带薪休假天数: %.1f/%.1f\n", emp->leave.paidLeaveDays, emp->leave.maxPaidLeaveDays);
}

// 显示所有员工
void displayAllEmployees() {
    if (employeeCount == 0) {
        printf("当前没有员工记录!\n");
        return;
    }
    
    printf("\n=== 所有员工信息 ===\n");
    for (int i = 0; i < employeeCount; i++) {
        displayEmployee(employees[i]);
        printf("-------------------\n");
    }
}

// 编辑员工
void editEmployee() {
    if (!currentUser.isAdmin) {
        printf("权限不足!\n");
        return;
    }
    
    if (employeeCount == 0) {
        printf("当前没有员工记录!\n");
        return;
    }
    
    char id[20];
    printf("输入要编辑的员工编号: ");
    scanf("%s", id);
    clearInputBuffer();
    
    int found = -1;
    for (int i = 0; i < employeeCount; i++) {
        if (strcmp(employees[i]->id, id) == 0) {
            found = i;
            break;
        }
    }
    
    if (found == -1) {
        printf("未找到该员工!\n");
        return;
    }
    
    Employee *emp = employees[found];
    printf("\n当前员工信息:\n");
    displayEmployee(emp);
    
    printf("\n选择要修改的字段:\n");
    printf("1. 姓名\n2. 性别\n3. 部门\n4. 级别\n5. 基本工资\n");
    if (strcmp(emp->department, "经理") == 0) {
        printf("6. 奖金\n");
    } else if (strcmp(emp->department, "技术") == 0) {
        printf("6. 加班费\n");
    } else if (strcmp(emp->department, "销售") == 0) {
        printf("6. 销售额\n7. 提成比例\n");
    } else if (strcmp(emp->department, "销售管理") == 0) {
        printf("6. 奖金\n7. 销售额\n8. 提成比例\n");
    }
    printf("选择: ");
    
    int choice;
    scanf("%d", &choice);
    clearInputBuffer();
    
    switch (choice) {
        case 1:
            printf("输入新姓名: ");
            scanf("%s", emp->name);
            strcpy(emp->leave.name, emp->name);
            break;
        case 2:
            printf("输入性别: ");
            scanf("%s", emp->gender);
            break;
        case 3:
            printf("输入部门: ");
            scanf("%s", emp->department);
            break;
        case 4:
            printf("输入级别: ");
            scanf("%d", &emp->level);
            break;
        case 5:
            printf("输入基本工资: ");
            scanf("%f", &emp->salary);
            break;
        case 6:
            if (strcmp(emp->department, "经理") == 0) {
                Manager *mgr = (Manager*)emp;
                printf("输入奖金: ");
                scanf("%f", &mgr->bonus);
            } else if (strcmp(emp->department, "技术") == 0) {
                Technician *tech = (Technician*)emp;
                printf("输入加班费: ");
                scanf("%f", &tech->overtimePay);
            } else if (strcmp(emp->department, "销售") == 0) {
                SalesPerson *sales = (SalesPerson*)emp;
                printf("输入销售额: ");
                scanf("%f", &sales->sales);
            } else if (strcmp(emp->department, "销售管理") == 0) {
                SalesManager *salesMgr = (SalesManager*)emp;
                printf("输入奖金: ");
                scanf("%f", &salesMgr->bonus);
            }
            break;
        case 7:
            if (strcmp(emp->department, "销售") == 0) {
                SalesPerson *sales = (SalesPerson*)emp;
                printf("输入提成比例: ");
                scanf("%f", &sales->commission);
            } else if (strcmp(emp->department, "销售管理") == 0) {
                SalesManager *salesMgr = (SalesManager*)emp;
                printf("输入销售额: ");
                scanf("%f", &salesMgr->sales);
            }
            break;
        case 8:
            if (strcmp(emp->department, "销售管理") == 0) {
                SalesManager *salesMgr = (SalesManager*)emp;
                printf("输入提成比例: ");
                scanf("%f", &salesMgr->commission);
            }
            break;
        default:
            printf("无效选择!\n");
            return;
    }
    
    printf("员工信息修改成功!\n");
}

// 删除员工
void deleteEmployee() {
    if (!currentUser.isAdmin) {
        printf("权限不足!\n");
        return;
    }
    
    if (employeeCount == 0) {
        printf("当前没有员工记录!\n");
        return;
    }
    
    char id[20];
    printf("输入要删除的员工编号: ");
    scanf("%s", id);
    clearInputBuffer();
    
    int found = -1;
    for (int i = 0; i < employeeCount; i++) {
        if (strcmp(employees[i]->id, id) == 0) {
            found = i;
            break;
        }
    }
    
    if (found == -1) {
        printf("未找到该员工!\n");
        return;
    }
    
    // 释放内存
    free(employees[found]);
    
    // 移动数组元素
    for (int i = found; i < employeeCount - 1; i++) {
        employees[i] = employees[i + 1];
    }
    
    employeeCount--;
    printf("员工删除成功!\n");
}

// 添加休假记录
void addLeaveRecord() {
    if (employeeCount == 0) {
        printf("当前没有员工记录!\n");
        return;
    }
    
    char id[20];
    printf("输入员工编号: ");
    scanf("%s", id);
    clearInputBuffer();
    
    int found = -1;
    for (int i = 0; i < employeeCount; i++) {
        if (strcmp(employees[i]->id, id) == 0) {
            found = i;
            break;
        }
    }
    
    if (found == -1) {
        printf("未找到该员工!\n");
        return;
    }
    
    int type;
    float hours;
    printf("选择休假类型:\n");
    printf("1. 病假\n2. 带薪休假\n");
    printf("选择: ");
    scanf("%d", &type);
    clearInputBuffer();
    
    if (type != 1 && type != 2) {
        printf("无效的休假类型!\n");
        return;
    }
    
    printf("输入休假小时数: ");
    scanf("%f", &hours);
    clearInputBuffer();
    
    if (hours <= 0) {
        printf("休假小时数必须大于0!\n");
        return;
    }
    
    TimeConverter tc = convertHoursToDays(hours);
    
    if (type == 1) { // 病假
        if (employees[found]->leave.sickLeaveDays + tc.days > 
            employees[found]->leave.maxSickLeaveDays) {
            printf("超过最大允许病假天数(%.1f天)!\n", 
                   employees[found]->leave.maxSickLeaveDays);
            return;
        }
        employees[found]->leave.sickLeaveDays += tc.days;
    } else { // 带薪休假
        if (employees[found]->leave.paidLeaveDays + tc.days > 
            employees[found]->leave.maxPaidLeaveDays) {
            printf("超过最大允许带薪休假天数(%.1f天)!\n", 
                   employees[found]->leave.maxPaidLeaveDays);
            return;
        }
        employees[found]->leave.paidLeaveDays += tc.days;
    }
    
    printf("休假记录添加成功!\n");
    printf("已添加 %.1f 小时 (%.1f 天) 的%s休假\n", 
           hours, tc.days, type == 1 ? "病假" : "带薪");
}

// 小时转换为天数
TimeConverter convertHoursToDays(float hours) {
    TimeConverter tc;
    tc.hours = hours;
    if (hours == 8) {
        tc.days = 1;
    } else if (hours == 12) {
        tc.days = 1.5;
    } else {
        tc.days = hours / 8; // 通用转换
    }
    return tc;
}

// 显示统计信息
void displayStatistics() {
    if (employeeCount == 0) {
        printf("当前没有员工记录!\n");
        return;
    }
    
    int managerCount = 0, techCount = 0, salesCount = 0, salesMgrCount = 0;
    float totalSalary = 0;
    
    for (int i = 0; i < employeeCount; i++) {
        if (strcmp(employees[i]->department, "经理") == 0) {
            managerCount++;
            Manager *mgr = (Manager*)employees[i];
            totalSalary += employees[i]->salary + mgr->bonus;
        } else if (strcmp(employees[i]->department, "技术") == 0) {
            techCount++;
            Technician *tech = (Technician*)employees[i];
            totalSalary += employees[i]->salary + tech->overtimePay;
        } else if (strcmp(employees[i]->department, "销售") == 0) {
            salesCount++;
            SalesPerson *sales = (SalesPerson*)employees[i];
            totalSalary += employees[i]->salary + sales->sales * sales->commission;
        } else if (strcmp(employees[i]->department, "销售管理") == 0) {
            salesMgrCount++;
            SalesManager *salesMgr = (SalesManager*)employees[i];
            totalSalary += employees[i]->salary + salesMgr->bonus + 
                          salesMgr->sales * salesMgr->commission;
        }
    }
    
    printf("\n=== 统计信息 ===\n");
    printf("经理人数: %d\n", managerCount);
    printf("技术人员人数: %d\n", techCount);
    printf("销售人员人数: %d\n", salesCount);
    printf("销售经理人数: %d\n", salesMgrCount);
    printf("员工总数: %d\n", employeeCount);
    printf("总工资支出: %.2f\n", totalSalary);
    
    // 查询特定员工休假信息
    printf("\n输入员工编号查询休假信息(直接回车跳过): ");
    char id[20];
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = '\0'; // 去除换行符
    
    if (strlen(id) > 0) {
        int found = -1;
        for (int i = 0; i < employeeCount; i++) {
            if (strcmp(employees[i]->id, id) == 0) {
                found = i;
                break;
            }
        }
        
        if (found == -1) {
            printf("未找到该员工!\n");
        } else {
            printf("\n员工 %s (%s) 的休假信息:\n", 
                   employees[found]->name, employees[found]->id);
            printf("病假天数: %.1f/%.1f\n", 
                   employees[found]->leave.sickLeaveDays, 
                   employees[found]->leave.maxSickLeaveDays);
            printf("带薪休假天数: %.1f/%.1f\n", 
                   employees[found]->leave.paidLeaveDays, 
                   employees[found]->leave.maxPaidLeaveDays);
        }
    }
}

// 保存数据到文件
void saveData() {
    FILE *empFile = fopen("employees.dat", "wb");
    FILE *userFile = fopen("users.dat", "wb");
    
    if (!empFile || !userFile) {
        printf("无法打开文件!\n");
        return;
    }
    
    // 保存员工数据
    fwrite(&employeeCount, sizeof(int), 1, empFile);
    for (int i = 0; i < employeeCount; i++) {
        // 根据员工类型保存不同的结构体
        if (strcmp(employees[i]->department, "经理") == 0) {
            Manager *mgr = (Manager*)employees[i];
            fwrite(mgr, sizeof(Manager), 1, empFile);
        } else if (strcmp(employees[i]->department, "技术") == 0) {
            Technician *tech = (Technician*)employees[i];
            fwrite(tech, sizeof(Technician), 1, empFile);
        } else if (strcmp(employees[i]->department, "销售") == 0) {
            SalesPerson *sales = (SalesPerson*)employees[i];
            fwrite(sales, sizeof(SalesPerson), 1, empFile);
        } else if (strcmp(employees[i]->department, "销售管理") == 0) {
            SalesManager *salesMgr = (SalesManager*)employees[i];
            fwrite(salesMgr, sizeof(SalesManager), 1, empFile);
        }
    }
    
    // 保存用户数据
    fwrite(&userCount, sizeof(int), 1, userFile);
    fwrite(users, sizeof(UserAccount), userCount, userFile);
    
    fclose(empFile);
    fclose(userFile);
    printf("数据保存成功!\n");
}

// 从文件加载数据
void loadData() {
    FILE *empFile = fopen("employees.dat", "rb");
    FILE *userFile = fopen("users.dat", "rb");
    
    if (!empFile) {
        // 如果员工文件不存在，创建一个空文件
        empFile = fopen("employees.dat", "wb");
        if (empFile) fclose(empFile);
        printf("员工数据文件不存在，已创建空文件。\n");
    } else {
        // 加载员工数据
        fread(&employeeCount, sizeof(int), 1, empFile);
        for (int i = 0; i < employeeCount; i++) {
            // 读取员工数据（根据类型加载）
            Employee temp;
            fread(&temp, sizeof(Employee), 1, empFile);
            // 根据部门确定类型并加载
            if (strcmp(temp.department, "经理") == 0) {
                Manager *mgr = (Manager*)malloc(sizeof(Manager));
                fseek(empFile, -sizeof(Employee), SEEK_CUR);
                fread(mgr, sizeof(Manager), 1, empFile);
                employees[i] = (Employee*)mgr;
            } else if (strcmp(temp.department, "技术") == 0) {
                Technician *tech = (Technician*)malloc(sizeof(Technician));
                fseek(empFile, -sizeof(Employee), SEEK_CUR);
                fread(tech, sizeof(Technician), 1, empFile);
                employees[i] = (Employee*)tech;
            } else if (strcmp(temp.department, "销售") == 0) {
                SalesPerson *sales = (SalesPerson*)malloc(sizeof(SalesPerson));
                fseek(empFile, -sizeof(Employee), SEEK_CUR);
                fread(sales, sizeof(SalesPerson), 1, empFile);
                employees[i] = (Employee*)sales;
            } else if (strcmp(temp.department, "销售管理") == 0) {
                SalesManager *salesMgr = (SalesManager*)malloc(sizeof(SalesManager));
                fseek(empFile, -sizeof(Employee), SEEK_CUR);
                fread(salesMgr, sizeof(SalesManager), 1, empFile);
                employees[i] = (Employee*)salesMgr;
            }
        }
        fclose(empFile);
    }

    if (!userFile) {
        // 如果用户文件不存在，创建一个空文件
        userFile = fopen("users.dat", "wb");
        if (userFile) fclose(userFile);
        printf("用户数据文件不存在，已创建空文件。\n");
    } else {
        // 加载用户数据
        fread(&userCount, sizeof(int), 1, userFile);
        fread(users, sizeof(UserAccount), userCount, userFile);
        fclose(userFile);
    }
}

// 清空输入缓冲区
void clearInputBuffer() {
    while (getchar() != '\n');
}