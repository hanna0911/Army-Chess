#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QPushButton>
#include <QRandomGenerator>
#include <QTimer>
#include <map>
#include <QtNetwork>
#include <QTime>
#include "serverpanel.h"
#include "clientpanel.h"
#include "invalidinput.h"
#include "invalidrequest.h"
#include "havewinner.h"
#include "offlinenotice.h"
#include "disconnectnotice.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
class QAction;
class QLabel;
class QMenu;
QT_END_NAMESPACE

class ChronoMarker;
class UnknownButton;
class ChessButton;
class MainWindow;


struct player{
    bool user; // server (User1) = true, client (User2) = false;
    int PreviousColor = -1; // before assign color
    int color; // red = 1, blue = 0 (after assigned color)
    int mine = 3;
    int exceedTime = 0;
};

struct board_coordinate{
    int x_loc;
    int y_loc;
};


// ChronoMarker

class ChronoMarker : public QObject{
    Q_OBJECT

public:
    ChronoMarker(MainWindow *parent = nullptr);
    int second;
    QTimer *timer;
    void startCount();

signals:
    void freshSignal(int s);

public slots:
    void on_timer_out();

};


// UnknownButton

class UnknownButton : public QPushButton{
    Q_OBJECT

public:
    UnknownButton(MainWindow *parent = nullptr);
    int id;
    MainWindow *window;

public slots:
    void switch_turn_unmask();
    void switch_turn_unmask_message();
};


// ChessButton

class ChessButton : public QPushButton{
    Q_OBJECT

public:
    ChessButton(MainWindow *parent = nullptr);
    int id; // total: 50
    MainWindow *window;
    int isRed;
    int ID; // 0.-11. (total: 12)
    int curNum;
    int maxNum;
    board_coordinate BtnLoc; // (i, j)
    bool alive = false;
    void initBtn(int, int, int, int);
    void select_chessBtn();
    void deselect_chessBtn(int);
    void select_emptyBtn();
    void deselect_emptyBtn();
    void operator=(const ChessButton &src){ // Reload operator=
        id = src.id;
        window = src.window;
        isRed = src.isRed;
        ID = src.ID;
        BtnLoc = src.BtnLoc;
    }

public slots:
    void press_own_chessBtn();
    void press_other_chessBtn();
    void press_emptyBtn();
    void press_emptyBtn_message();
    void press_chessBtn_message();
};


// MainWindow

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    Ui::MainWindow *ui;
    ChessButton *chessBtn[51]; // place on layer 1
    ChessButton *emptyBtn[61]; // place on bottom layer 0
    std::map<int, int> precise_estimate_x; // map[x] = i;
    std::map<int, int> precise_estimate_y; // map[y] = j;
    player currentPlayer;
    player nextPlayer;
    int countDown = 20;
    QString user1 = "User1"; // Can add new function [type in own username]
    QString user2 = "User2";
    bool assignedSide = false;
    int clicked_own_chessBtn = 0;
    int clicked_other_chessBtn = 0;
    int clicked_emptyBtn = 0;
    ChessButton preclicked_chessBtn; // Incomplete Info for ChessButton (hopefully enough)
    ChessButton curclicked_chessBtn; // Incomplete Info for ChessButton (hopefully enough)
    bool needMove = false;
    int unknown = 50; // counting unknown pieces
    bool haveChess[61] = {};
    QString messageSent = ""; // send message
    QString messageRecv = ""; // receive message
    QTcpServer *listenSocket; // Server
    QTcpSocket *readWriteSocket; // Server & Client
    std::map<int, char> int_to_char;
    std::map<char, int> char_to_int;
    void swapPlayers();
    bool validPath();
    bool validEat(bool);
    bool isSafe(board_coordinate); // hangying / xingying
    void Move();  
    bool checkValid(QString);
    ~MainWindow();

private:
    ChronoMarker *clock;
    bool generatedChesspieces = false;
    bool end_game = false;
    int matrix[61][61] = {};
    bool isUser1;
    int randomArray[51] = {};
    bool errorOccurred;
    bool startedServerConnect = false;
    UnknownButton *unknownBtn[51]; // place on layer 2, close after clicked
    QRect *location[13][6];
    std::map<int, QRect> init_num_loc;
    std::map<int, board_coordinate> whole_num_loc;
    int whole_loc_num[13][6];
    bool ableStart_self = false, ableStart_else = false; // After pressed startAct, server & client can then start; have to send messages to each other, and change "else" status ("self" status changed by pressing startAct)
    bool started = false;
    int step = 0;
    int round = 0;
    int randNum = -1;
    QTimer *checkState;
    void createActions();
    void generateChesspieces();
    void placeChesspieces();
    void generateLocation();
    void init_mapLoc();
    void whole_mapLoc();
    void construct_matrix();
    bool onRailway(int);
    void endGame(player);
    bool sbWins();
    bool noChessToMove();
    bool send_WakeHand();

public slots:
    void show_clock(int s);
    void acceptConnection(); // Server
    void recvMessage(); // Server & Client
    void sendMessage(); // Server & Client

private slots:
    void open_server_panel();
    void open_client_panel();
    void start();
    void admit_defeat();
    void slotConnected();
    void update_DeviceNetState();
    void disconnect();
};

#endif // MAINWINDOW_H
