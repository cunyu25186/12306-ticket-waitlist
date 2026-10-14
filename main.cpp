#include <iostream>
#include <string>
#include <queue>
#include <iomanip>

using namespace std;

// 1. 候补订单结构体（对应 PPT 第 3 页）
struct CandidateOrder {
    string orderNo;        // 订单号
    string passengerName;  // 旅客姓名
    string idCard;         // 身份证号
    int ticketCount;       // 候补票数
};

// 全局候补队列（FIFO，对应 PPT 第 3 页）
queue<CandidateOrder> waiting;

// 打印主菜单（对应 PPT 第 4 页）
void showMenu() {
    cout << "\n==========================================" << endl;
    cout << "     12306 车票候补系统（基础版）         " << endl;
    cout << "==========================================" << endl;
    cout << "  1. 新增候补订单" << endl;
    cout << "  2. 释放余票（候补兑现）" << endl;
    cout << "  3. 查看候补队列" << endl;
    cout << "  0. 退出程序" << endl;
    cout << "==========================================" << endl;
    cout << "请选择操作编号: ";
}

// 操作 1：新增候补订单（对应 PPT 第 4 页 操作 A）
void addOrder() {
    CandidateOrder order;
    cout << "\n--- [新增候补订单] ---" << endl;
    cout << "请输入订单号: ";
    cin >> order.orderNo;
    cout << "请输入旅客姓名: ";
    cin >> order.passengerName;
    cout << "请输入身份证号: ";
    cin >> order.idCard;
    cout << "请输入候补票数: ";
    cin >> order.ticketCount;

    // 输入合法性检查
    if (order.ticketCount <= 0) {
        cout << "【错误】候补票数必须大于 0，订单录入失败！" << endl;
        return;
    }

    // 新订单加入队尾（push）
    waiting.push(order);
    cout << "【成功】订单 " << order.orderNo << " 已加入候补队列！" << endl;
    cout << "当前队列总等待人数: " << waiting.size() << " 人。" << endl;
}

// 操作 2：释放余票并兑现（对应 PPT 第 4、5 页 核心算法流程图）
void releaseTickets() {
    cout << "\n--- [释放余票与候补兑现] ---" << endl;
    int k = 0;
    cout << "请输入本次释放的余票数量 k: ";
    cin >> k;

    // 输入合法性校验
    if (k <= 0) {
        cout << "【提示】释放票数必须为正整数！" << endl;
        return;
    }

    if (waiting.empty()) {
        cout << "【提示】当前候补队列为空，无需分配，余票直接进入售票池。" << endl;
        return;
    }

    int remaining = k;  // 当前剩余可用票数
    int successCount = 0; // 本轮成功兑现订单数

    cout << "\n>>> 开始从队头依次处理候补订单 >>>" << endl;

    // 核心算法循环：队列非空 且 remaining > 0（完全对应 PPT 第 5 页流程图）
    while (!waiting.empty() && remaining > 0) {
        // 取出队头订单
        CandidateOrder order = waiting.front();

        // 关键判断：队头所需票数是否超出剩余票数？
        if (order.ticketCount > remaining) {
            // 【核心原则：不可跳过队头】
            cout << "------------------------------------------" << endl;
            cout << "【暂停分配】队头订单 [" << order.orderNo << " | " << order.passengerName 
                 << "] 需要 " << order.ticketCount << " 张票，但当前仅剩 " 
                 << remaining << " 张票。" << endl;
            cout << "为保证公平排队原则，系统不能跳过队头！本轮兑现结束，队头继续保留等待。" << endl;
            break; // 停止本轮处理
        }

        // 票数充足，成功兑现该订单
        cout << "------------------------------------------" << endl;
        cout << "【候补成功】订单号: " << order.orderNo 
             << " | 旅客: " << order.passengerName 
             << " | 身份证: " << order.idCard 
             << " | 成功出票: " << order.ticketCount << " 张！" << endl;

        // 扣减余票
        remaining -= order.ticketCount;
        successCount++;

        // 队头订单出队
        waiting.pop();
    }

    // 输出本次放票的总结结果
    cout << "------------------------------------------" << endl;
    cout << ">>> 本轮放票处理完毕 <<<" << endl;
    cout << "本次共释放: " << k << " 张 | 实际兑现: " << (k - remaining) 
         << " 张 | 最终剩余: " << remaining << " 张。" << endl;
    cout << "本轮共兑现成功订单: " << successCount << " 笔。" << endl;
    cout << "候补队列中剩余等待订单: " << waiting.size() << " 笔。" << endl;
}

// 操作 3：查看候补队列（对应 PPT 第 4 页 操作 C / 辅助功能）
void viewQueue() {
    cout << "\n--- [当前候补队列状态] ---" << endl;
    if (waiting.empty()) {
        cout << "当前候补队列为空，暂无排队旅客。" << endl;
        return;
    }

    // 利用队列副本进行顺序遍历展示，不破坏原队列
    queue<CandidateOrder> tempQueue = waiting;
    int rank = 1;

    cout << left << setw(6)  << "顺位" 
         << left << setw(12) << "订单号" 
         << left << setw(10) << "姓名" 
         << left << setw(20) << "身份证号" 
         << left << setw(8)  << "所需票数" << endl;
    cout << "--------------------------------------------------------" << endl;

    while (!tempQueue.empty()) {
        CandidateOrder order = tempQueue.front();
        tempQueue.pop();

        string tag = "";
        if (rank == 1) tag = " (队头)";

        cout << left << setw(6)  << (to_string(rank) + tag)
             << left << setw(12) << order.orderNo 
             << left << setw(10) << order.passengerName 
             << left << setw(20) << order.idCard 
             << left << setw(8)  << order.ticketCount << endl;
        rank++;
    }
    cout << "--------------------------------------------------------" << endl;
    cout << "总计等待订单数: " << waiting.size() << endl;
}

int main() {
    // 提高输入输出效率
    ios::sync_with_stdio(0);
    cin.tie(0);

    int choice = -1;
    while (true) {
        showMenu();
        if (!(cin >> choice)) {
            // 处理非数字输入的异常情况
            cin.clear();
            string discard;
            cin >> discard;
            cout << "【输入错误】请输入有效的数字选项！" << endl;
            continue;
        }

        switch (choice) {
            case 1:
                addOrder();
                break;
            case 2:
                releaseTickets();
                break;
            case 3:
                viewQueue();
                break;
            case 0:
                cout << "\n感谢使用 12306 车票候补系统，系统已退出。" << endl;
                return 0;
            default:
                cout << "【无效选项】没有此功能，请重新输入！" << endl;
                break;
        }
    }

    return 0;
}
