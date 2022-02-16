#include "mainwindow.h"
#include "ui_mainwindow.h"


// ChronoMarker

ChronoMarker::ChronoMarker(MainWindow *parent) : QObject(parent){
    second = parent->countDown;
    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(on_timer_out()));
    timer->start();
}

void ChronoMarker::on_timer_out(){
    second--;
    emit freshSignal(second);
}


//UnknownButton

UnknownButton::UnknownButton(MainWindow *parent) : QPushButton(parent){
    window = parent;
}

void UnknownButton::switch_turn_unmask(){
    // Unmask Chesspiece
    close();
    window->unknown--;

    // Assign Side
    if(!window->assignedSide){

        if(window->currentPlayer.PreviousColor == window->chessBtn[id]->isRed){ // if: preColor == curColor
            window->assignedSide = true;

            // Set User's color
            window->currentPlayer.color = window->chessBtn[id]->isRed;
            window->nextPlayer.color = 1 - window->chessBtn[id]->isRed;

            // Update Color Info
            std::map<int, QString> color = {{1, "Red"}, {0, "Blue"}};
            QString colorInfoText = "";
            if(window->currentPlayer.user == true)
                colorInfoText = color[window->chessBtn[id]->isRed] + "\n" + color[1 - window->chessBtn[id]->isRed];
            else colorInfoText = color[1 - window->chessBtn[id]->isRed] + "\n" + color[window->chessBtn[id]->isRed];
            window->ui->colorInfoLb->setText(colorInfoText);
        }
        else window->currentPlayer.PreviousColor = window->chessBtn[id]->isRed;
    }

    // switch players + restart clock
    window->show_clock(-101);

}

void UnknownButton::switch_turn_unmask_message(){
    window->messageSent = "switch_turn_unmask";
    window->messageSent += window->int_to_char[id];
    window->sendMessage();
}


// ChessButton

ChessButton::ChessButton(MainWindow *parent) : QPushButton(parent){
    window = parent;
}

void ChessButton::initBtn(int a, int b, int c, int d) {
    isRed = a;
    ID = b;
    curNum = c;
    maxNum = d;
    QString fileName = "ChessButton{border-image:url(:/chesspieces/";
    fileName += QString::number(isRed);
    if(ID <= 9){
        fileName += "0";
        fileName += QString::number(ID);
    }
    else fileName += QString::number(ID);
    fileName += ".png);}";
    setStyleSheet(fileName);
}

void ChessButton::select_chessBtn(){
    QString selectedStyle = "ChessButton{image:url(:/chesspieces/";
    selectedStyle += QString::number(isRed);
    if(ID <= 9) selectedStyle += "0";
    selectedStyle += QString::number(ID);
    selectedStyle += ".png);}ChessButton{border:2px solid #ffff00}";
    setStyleSheet(selectedStyle);
}

void ChessButton::deselect_chessBtn(int x){
    QString fileName = "ChessButton{border-image:url(:/chesspieces/";
    fileName += QString::number(window->chessBtn[x]->isRed);
    if(window->chessBtn[x]->ID <= 9){
        fileName += "0";
        fileName += QString::number(window->chessBtn[x]->ID);
    }
    else fileName += QString::number(window->chessBtn[x]->ID);
    fileName += ".png);}";
    window->chessBtn[x]->setStyleSheet(fileName);
}

void ChessButton::select_emptyBtn(){
    QString selectedStyle = "ChessButton{image:url(:/other/Transparent.PNG);}ChessButton{border:2px solid #ffff00}";
    setStyleSheet(selectedStyle);
}

void ChessButton::deselect_emptyBtn(){
    QString fileName = "ChessButton{border-image:url(:/other/Transparent.PNG);}";
    setStyleSheet(fileName);
}

void ChessButton::press_own_chessBtn(){
    // Check if ChessBtn Can be pressed (restrictions on color + ID)
    if(window->assignedSide){

        if(window->currentPlayer.color == isRed && ID >= 1 && ID <= 10){ // check if valid color pressed

            // "Real enter [press_own_chessBtn]"
            window->clicked_own_chessBtn++;

            // deselect previous ChessBtn
            if(window->clicked_own_chessBtn >= 2){
                if(window->preclicked_chessBtn.id != id){
                    select_chessBtn(); // Highlight ChessBtn Selected
                    deselect_chessBtn(window->preclicked_chessBtn.id); // Deselect Previous Selected
                    window->clicked_own_chessBtn = 1;
                    window->preclicked_chessBtn = *this;
                }
                else{
                    deselect_chessBtn(window->preclicked_chessBtn.id); // Deselect Self
                    window->clicked_own_chessBtn = 0;
                    window->preclicked_chessBtn.id = -1; // dangerous
                }
            }
            else{
                select_chessBtn(); // Select Self, 1st time
                window->preclicked_chessBtn = *this;
            }
        }
    }
    // "Haven't assigned sides yet, can't push self!"
}

void ChessButton::press_other_chessBtn(){
    if(window->clicked_other_chessBtn == 0){

        if(window->currentPlayer.color != isRed){ // Check if Chessbtn can be pressed (opposite color)

            // Real enter [press_other_chessBtn]
            window->clicked_other_chessBtn++;

            if(window->clicked_own_chessBtn > 0){
                window->curclicked_chessBtn = *this; // set curclicked_chessBtn
                select_chessBtn(); // Highlight ChessBtn Selected

                if(window->validPath()){
                    if(window->validEat(true)){
                        if(window->needMove){
                            window->Move();
                        }
                        deselect_chessBtn(window->preclicked_chessBtn.id); // Deselect Own Chess
                        window->show_clock(-101);
                    }
                    else{
                        // Other ChessBtn Can't Be Eaten! Reselect Both
                        deselect_chessBtn(window->preclicked_chessBtn.id); // Deselect Own Chess
                        deselect_chessBtn(window->curclicked_chessBtn.id); // Deselect Other Chess
                        window->clicked_own_chessBtn = 0;
                        window->clicked_other_chessBtn = 0;
                        window->preclicked_chessBtn.id = -1;
                        window->curclicked_chessBtn.id = -1;
                    }
                }
                else{
                    // Other ChessBtn Can't Be Eaten! Reselect Both
                    deselect_chessBtn(window->preclicked_chessBtn.id); // Deselect Own Chess
                    deselect_chessBtn(window->curclicked_chessBtn.id); // Deselect Other Chess
                    window->clicked_own_chessBtn = 0;
                    window->clicked_other_chessBtn = 0;
                    window->preclicked_chessBtn.id = -1;
                    window->curclicked_chessBtn.id = -1;
                }
            }
            else{
                window->clicked_other_chessBtn = 0; // "Haven't picked own chessBtn yes!"
            }
        }
    }
}

void ChessButton::press_chessBtn_message(){
    window->messageSent = "press_chessBtn";
    window->messageSent += window->int_to_char[id];
    window->sendMessage();
}

void ChessButton::press_emptyBtn(){
    if(window->clicked_emptyBtn == 0){
        window->clicked_emptyBtn++;
        if(window->clicked_own_chessBtn > 0){
            window->curclicked_chessBtn = *this; // set curclicked_chessBtn
            select_emptyBtn(); // Highlight EmptyBtn Selected
            if(window->validPath()){
                window->Move();

                // Deselect Both;
                deselect_chessBtn(window->preclicked_chessBtn.id); // Deselect Own Chess
                deselect_emptyBtn(); // Deselect Empty Chess
                window->show_clock(-101);
            }
            else{
                // Deselect Both;
                deselect_chessBtn(window->preclicked_chessBtn.id); // Deselect Own Chess
                deselect_emptyBtn(); // Deselect Empty Chess

                // Can't Move! Reselect Both
                window->clicked_own_chessBtn = 0;
                window->clicked_emptyBtn = 0;
                window->preclicked_chessBtn.id = -1;
                window->curclicked_chessBtn.id = -1;
            }
        }
        else window->clicked_emptyBtn = 0; // Haven't picked own chessBtn yes!"
    }
}

void ChessButton::press_emptyBtn_message(){
    window->messageSent = "press_emptyBtn";
    window->messageSent += window->int_to_char[id];
    window->sendMessage();
}


// MainWindow

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createActions();

    QString userInfoText = user1;
    userInfoText += "\n";
    userInfoText += user2;
    ui->userInfoLb->setText(userInfoText);

    preclicked_chessBtn.id = -1; // dangerous
    curclicked_chessBtn.id = -1;

    whole_mapLoc(); // generate No. -> (i, j) map
    construct_matrix(); // construct matrix[60][60] (road: 1, railway: 2)

    // create mapping
    for(int i = 1; i <= 60; i++){
        int_to_char.insert({i, i});
        char_to_int.insert({i, i});
    }
}

// Server
void MainWindow::acceptConnection(){
    // Adapted from teacher's code
    readWriteSocket = listenSocket->nextPendingConnection();
    connect(readWriteSocket, SIGNAL(readyRead()), this, SLOT(recvMessage()));
}

// Server & Client
void MainWindow::recvMessage(){
    messageRecv = "";
    messageRecv += readWriteSocket->readAll(); // Adapted from teacher's code

    // Tons of IF-ELSEIF-ELSE coming~
    if(messageRecv.startsWith("actionStart")){ // Start game after both pressed actionStart & generate random chesspieces
        if(messageRecv.startsWith("actionStart_1")){
            ableStart_else = true;

            // Sycn randNum
            if(messageRecv.startsWith("actionStart_11")) randNum = 1;
            else randNum = 0; // messageRecv == "actionStart_10"

            // Set to user2
            isUser1 = false;

            // Receive random array
            for(int i = 1; i <= 50; i++){
                randomArray[i] = char_to_int[messageRecv[i + 13].toLatin1()];
            }
        }
        else{ // messageRecv == "actionStart_2"
            ableStart_else = true;
            start();
        }
    }
    else if(messageRecv.startsWith("switch_turn_unmask")){ // pressed unknown button
        int id = char_to_int[messageRecv[18].toLatin1()];
        unknownBtn[id]->switch_turn_unmask();
    }
    else if(messageRecv.startsWith("press_chessBtn")){ // pressed chess button
        int id = char_to_int[messageRecv[14].toLatin1()];
        chessBtn[id]->press_own_chessBtn();
        chessBtn[id]->press_other_chessBtn();
    }
    else if(messageRecv.startsWith("press_emptyBtn")){ // pressed empty button
        int id = char_to_int[messageRecv[14].toLatin1()];
        emptyBtn[id]->press_emptyBtn();
    }

    else if(messageRecv.startsWith("admit_defeat")){
        if(messageRecv == "admit_defeatnext"){
            endGame(nextPlayer);
            show_clock(-101);
        }
        else{
            endGame(currentPlayer);
            show_clock(-101);
        }
    }
}

// Server & Client
void MainWindow::sendMessage(){
    // Adapted from teacher's code
    QByteArray *array = new QByteArray;
    array->clear();
    array->append(messageSent.toUtf8());
    readWriteSocket->write(array->data());
}

void MainWindow::open_server_panel(){
    ServerPanel *w = new ServerPanel(this);

    // Start up Server Connection
    if(!startedServerConnect){ // Only start up once
        listenSocket = new QTcpServer;
        listenSocket->listen(QHostAddress::Any, 6666);

        connect(listenSocket, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
        connect(listenSocket, SIGNAL(newConnection()), this, SLOT(slotConnected()));

        startedServerConnect = true;
    }

    if(w->exec() == QDialog::Accepted){
        if(!generatedChesspieces){
            generateChesspieces();
            generatedChesspieces = true;
        }
    }
}

bool MainWindow::send_WakeHand(){
    int ret = readWriteSocket->write("");
    if(ret == -1)
        return false; // offline
    return true; // online
}

void MainWindow::update_DeviceNetState(){
    bool netState = send_WakeHand();
    if(netState == false){
        // offline notice
        checkState->stop();

        OfflineNotice *w = new OfflineNotice(this);
        w->show();

        end_game = true;
        show_clock(-101);
        QString showText = "---------";
        ui->turnLb->setText(showText);
        ui->timerLCD->display(88);

        // Renew Clicked Status
        for(int i = 1; i <= 50; i++){
            chessBtn[1]->deselect_chessBtn(i);
        }

        // Unable buttons and Menu
        for(int i = 1; i <= 50; i++){
            unknownBtn[i]->setEnabled(false);
            chessBtn[i]->setEnabled(false);
        }
        for(int i = 1; i <= 60; i++){
            emptyBtn[i]->setEnabled(false);
        }
        ui->actionAdmit_the_defeat->setEnabled(false);
    }
}

bool MainWindow::checkValid(QString clientIP){
    QRegularExpression regx("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b"); // QRegularExpression regx("((2(5[0-5]|[0-4]d))|[0-1]?d{1,2})(.((2(5[0-5]|[0-4]d))|[0-1]?d{1,2})){3}"); // failed // QRegularExpression regx("^([0-9]{1,3}[.]{1}[0-9]{1,3}[.]{1}[0-9]{1,3}[.]{1}[0-9]{1,3})$"); // xxx.xxx.xxx.xxx
    QRegularExpressionValidator val(regx, 0);
    int pos = 0;
    QValidator::State res;
    res = val.validate(clientIP, pos);
    if(QValidator::Acceptable != res) return false;
    else{
        // Connect to Server
        readWriteSocket = new QTcpSocket;
        readWriteSocket->connectToHost(QHostAddress(clientIP), 6666);

        connect(readWriteSocket, SIGNAL(readyRead()), this, SLOT(recvMessage()));
        connect(readWriteSocket, SIGNAL(connected()), this, SLOT(slotConnected()));

        // Wait 1 second for connected() signal to emit
        QTime dieTime = QTime::currentTime().addMSecs(1000);
        while( QTime::currentTime() < dieTime)
          QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

        return true;
    }
}

void MainWindow::slotConnected(){
    errorOccurred = false;
    ui->actionStart->setEnabled(true);
    ui->actionConnect_to_Server->setEnabled(false);
    ui->actionCreate_the_connection->setEnabled(false);
    ui->actionDisconnect->setEnabled(true);

    checkState = new QTimer(this);
    connect(checkState, SIGNAL(timeout()), this, SLOT(update_DeviceNetState()));
    checkState->start(1000);
}

void MainWindow::open_client_panel(){
    ClientPanel *w = new ClientPanel(this);
    bool closeClientPanel = false;
    while(!closeClientPanel && w->exec() == QDialog::Accepted){
        errorOccurred = true;
        if(checkValid(w->returnIP()) && !errorOccurred){
            if(!generatedChesspieces){
                generateChesspieces();
                generatedChesspieces = true;
            }
            closeClientPanel = true;
        }
        else{
            InvalidInput *v = new InvalidInput();
            if(v->exec() == QDialog::Rejected){
                closeClientPanel = true;
            }
        }
    }
}

void MainWindow::swapPlayers(){
    player tempPlayer = currentPlayer;
    currentPlayer = nextPlayer;
    nextPlayer = tempPlayer;
}

void MainWindow::Move(){
    // Previous Spot: pre, Destination: cur

    // Renew Empty Chess Status
    emptyBtn[whole_loc_num[preclicked_chessBtn.BtnLoc.x_loc][preclicked_chessBtn.BtnLoc.y_loc]]->alive = true;
    emptyBtn[whole_loc_num[curclicked_chessBtn.BtnLoc.x_loc][curclicked_chessBtn.BtnLoc.y_loc]]->alive = false;

    chessBtn[preclicked_chessBtn.id]->setGeometry(*location[(curclicked_chessBtn.BtnLoc).x_loc][(curclicked_chessBtn.BtnLoc).y_loc]); // Get coordinate (i, j) of Destination
    haveChess[whole_loc_num[chessBtn[preclicked_chessBtn.id]->BtnLoc.x_loc][chessBtn[preclicked_chessBtn.id]->BtnLoc.y_loc]] = false;
    chessBtn[preclicked_chessBtn.id]->BtnLoc = {(curclicked_chessBtn.BtnLoc).x_loc, (curclicked_chessBtn.BtnLoc).y_loc}; // Renew ChessBtn Location
    haveChess[whole_loc_num[chessBtn[preclicked_chessBtn.id]->BtnLoc.x_loc][chessBtn[preclicked_chessBtn.id]->BtnLoc.y_loc]] = true;
}

void MainWindow::whole_mapLoc(){
    int t = 0;
    for(int i = 1; i <= 12; i++){
        for(int j = 1; j <= 5; j++){
            t++;
            whole_num_loc.insert(std::pair<int, board_coordinate>(t, {i, j}));
            whole_loc_num[i][j] = t;
        }
    }
}

void MainWindow::construct_matrix(){
    for(int i = 1; i <= 59; i++){
        if(i % 5 == 0){
            matrix[i][i + 1] = 0;
            matrix[i + 1][i] = 0;
        }
        else if((6 <= i && i <= 9) || (26 <= i && i <= 34) || (51 <= i && i <= 54)){
            matrix[i][i + 1] = 2;
            matrix[i + 1][i] = 2;
        }
        else{
            matrix[i][i + 1] = 1;
            matrix[i + 1][i] = 1;
        }
    }
    for(int i = 1; i <= 55; i++){
        if((i % 5 == 0 && i != 5 && i != 55) || i == 6 || i == 11 || i == 16 || i == 21 || i == 26 || i == 28 || i == 31 || i == 36 || i == 41 || i == 46){
            matrix[i][i + 5] = 2;
            matrix[i + 5][i] = 2;
        }
        else if(i == 27 || i == 29){
            matrix[i][i + 5] = 0;
            matrix[i + 5][i] = 0;
        }
        else{
            matrix[i][i + 5] = 1;
            matrix[i + 5][i] = 1;
        }
    }
    for(int i = 6; i <= 24; i = i + 2){
        if((6 <= i && i <= 8) || (12 <= i && i <= 18) || (22 <= i && i <= 24)){
            matrix[i][i + 6] = 1;
            matrix[i + 6][i] = 1;
        }
    }
    for(int i = 31; i <= 49; i = i + 2){
        if((31 <= i && i <= 33) || (37 <= i && i <= 43) || (47 <= i && i <= 49)){
            matrix[i][i + 6] = 1;
            matrix[i + 6][i] = 1;
        }
    }
    for(int i = 8; i <= 24; i = i + 2){
        matrix[i][i + 4] = 1;
        matrix[i + 4][i] = 1;
    }
    matrix[16][20] = 0;
    matrix[20][16] = 0;
    for(int i = 33; i <= 49; i = i + 2){
        matrix[i][i + 4] = 1;
        matrix[i + 4][i] = 1;
    }
    matrix[41][45] = 0;
    matrix[45][41] = 0;
}

bool MainWindow::onRailway(int point){
    for(int i = 1; i <= 60; i++){
        if(matrix[point][i] == 2) return true;
    }
    return false;
}

int min(int a, int b){
    if(a < b) return a;
    return b;
}

int max(int a, int b){
    if(a > b) return a;
    return b;
}

bool MainWindow::validPath(){
    // yours: pre (x1, y1), other: cur (x2, y2)
    int x1 = (preclicked_chessBtn.BtnLoc).x_loc;
    int y1 = (preclicked_chessBtn.BtnLoc).y_loc;
    int x2 = (curclicked_chessBtn.BtnLoc).x_loc;
    int y2 = (curclicked_chessBtn.BtnLoc).y_loc;
    int point1 = whole_loc_num[x1][y1]; // point 1
    int point2 = whole_loc_num[x2][y2]; // point 2
    if(onRailway(point1) && onRailway(point2)){ // Both On Railway
        if(preclicked_chessBtn.ID == 1){ // You: Soldier, can take turns on railway
            // BFS
            std::list<int> waitingList;
            bool canGo[61] = {};
            // put yourself in
            waitingList.insert(waitingList.end(), point1);
            canGo[point1] = true;
            while(!waitingList.empty()){
                // Put near ones (new ones) in List, once put, remove self
                int checkPoint = *waitingList.begin();
                for(int i = 1; i <= 60; i++){
                    if(matrix[checkPoint][i] == 2 && (!haveChess[i] || i == point2) && !canGo[i]){ // added i == point2
                        canGo[i] = true;
                        waitingList.insert(waitingList.end(), i);
                    }
                }
                waitingList.pop_front();
            }

            if(canGo[point2]) return true; // Check if can get to point 2
            else return false;
        }
        else{ // You're not Soldier, can only walk in straight lines (lines should have railway = 2!)
            if(x1 == x2 || y1 == y2){
                if(x1 == x2){ // On same x-axis
                    for(int y = min(y1, y2) + 1; y <= max(y1, y2) - 1; y++){ // for any x = x1 = x2, y1 < y <= y2: no ChessButton on (x, y)
                        if(haveChess[whole_loc_num[x1][y]] || matrix[whole_loc_num[x1][y - 1]][whole_loc_num[x1][y]] != 2){
                            return false;
                        }
                    }
                    if(matrix[whole_loc_num[x1][max(y1, y2) - 1]][whole_loc_num[x1][max(y1, y2)]] != 2) return false;
                    return true;
                }
                else{ // On same y-axis
                    for(int x = min(x1, x2) + 1; x <= max(x1, x2) - 1; x++){ // for any y = y1 = y2, x1 < x <= x2: no ChessButton on (x, y)
                        if(haveChess[whole_loc_num[x][y1]] || matrix[whole_loc_num[x - 1][y1]][whole_loc_num[x][y1]] != 2){
                            return false;
                        }
                    }
                    if(matrix[whole_loc_num[max(x1, x2) - 1][y1]][whole_loc_num[max(x1, x2)][y1]] != 2) return false;
                    return true;
                }
            }
            else return false;
        }
    }
    else{ // Go through Road
        // you: point1(x1, y1), other: point2(x2, y2)
        // (x1, y1), (x2, y2) are 8-dir near (have road path = 1)
        if(matrix[point1][point2] == 1) return true;
        else return false;
    }
}

bool MainWindow::isSafe(board_coordinate point){
    int num = whole_loc_num[point.x_loc][point.y_loc];
    if(num == 12 || num == 14 || num == 18 || num == 22 || num == 24 || num == 37 || num == 39 || num == 43 || num == 47 || num == 49)
        return true;   
    return false;
}

// only contains compare of ID, doesn't contain path validation
bool MainWindow::validEat(bool isReal){
    // yours: pre, other: cur

    // other in Safety Place
    if(isSafe(curclicked_chessBtn.BtnLoc)) return false;

    // other Not in Safety Place
    if((preclicked_chessBtn.ID == 10 && curclicked_chessBtn.ID != 11) || curclicked_chessBtn.ID == 10 || preclicked_chessBtn.ID == curclicked_chessBtn.ID){ // (You: Bomb & Other NOT flag) or Other: Bomb or Same
        // Both Die
        if(isReal){
            chessBtn[preclicked_chessBtn.id]->hide();
            chessBtn[curclicked_chessBtn.id]->hide();
            chessBtn[preclicked_chessBtn.id]->alive = false;
            chessBtn[curclicked_chessBtn.id]->alive = false;

            // Renew Empty Chess Status
            emptyBtn[whole_loc_num[preclicked_chessBtn.BtnLoc.x_loc][preclicked_chessBtn.BtnLoc.y_loc]]->alive = true;
            emptyBtn[whole_loc_num[curclicked_chessBtn.BtnLoc.x_loc][curclicked_chessBtn.BtnLoc.y_loc]]->alive = true;

            haveChess[whole_loc_num[chessBtn[preclicked_chessBtn.id]->BtnLoc.x_loc][chessBtn[preclicked_chessBtn.id]->BtnLoc.y_loc]] = false;
            haveChess[whole_loc_num[chessBtn[curclicked_chessBtn.id]->BtnLoc.x_loc][chessBtn[curclicked_chessBtn.id]->BtnLoc.y_loc]] = false;

            if(curclicked_chessBtn.ID == 0) nextPlayer.mine--; // Other: Mine
            needMove = false;
        }
        return true;
    }
    else if(preclicked_chessBtn.ID == 1){ // You: Soldier
        if(curclicked_chessBtn.ID == 0){ // Other: Mine
            // Other Mine Die
            if(isReal){
                chessBtn[curclicked_chessBtn.id]->hide();
                chessBtn[curclicked_chessBtn.id]->alive = false;

                haveChess[whole_loc_num[chessBtn[curclicked_chessBtn.id]->BtnLoc.x_loc][chessBtn[curclicked_chessBtn.id]->BtnLoc.y_loc]] = false;
                nextPlayer.mine--;
                needMove = true;
            }
            return true;
        }
        else if(curclicked_chessBtn.ID == 11){ // Other: Flag
            if(nextPlayer.mine == 0){
                // Other Flag Die
                if(isReal){
                    chessBtn[curclicked_chessBtn.id]->hide();
                    chessBtn[curclicked_chessBtn.id]->alive = false;
                    haveChess[whole_loc_num[chessBtn[curclicked_chessBtn.id]->BtnLoc.x_loc][chessBtn[curclicked_chessBtn.id]->BtnLoc.y_loc]] = false;
                    needMove = true;
                    endGame(currentPlayer); // not necessarilly here (set a [check_endGame()] and call every step)
                }
                return true;
            }
            else return false;
        }
        else return false;
    }
    else if(curclicked_chessBtn.ID == 11){ // Other: Flag
        if(nextPlayer.mine == 0){
            if(preclicked_chessBtn.ID == 10){ // Both Die
                if(isReal){
                    chessBtn[preclicked_chessBtn.id]->hide();
                    chessBtn[curclicked_chessBtn.id]->hide();
                    chessBtn[preclicked_chessBtn.id]->alive = false;
                    chessBtn[curclicked_chessBtn.id]->alive = false;

                    // Renew Empty Chess Status
                    emptyBtn[whole_loc_num[preclicked_chessBtn.BtnLoc.x_loc][preclicked_chessBtn.BtnLoc.y_loc]]->alive = true;
                    emptyBtn[whole_loc_num[curclicked_chessBtn.BtnLoc.x_loc][curclicked_chessBtn.BtnLoc.y_loc]]->alive = true;

                    haveChess[whole_loc_num[chessBtn[preclicked_chessBtn.id]->BtnLoc.x_loc][chessBtn[preclicked_chessBtn.id]->BtnLoc.y_loc]] = false;
                    haveChess[whole_loc_num[chessBtn[curclicked_chessBtn.id]->BtnLoc.x_loc][chessBtn[curclicked_chessBtn.id]->BtnLoc.y_loc]] = false;

                    needMove = false;
                    endGame(currentPlayer); // not necessarilly here (set a [check_endGame()] and call every step)
                }
                return true;
            }
            else{ // Other Flag Die
                if(isReal){
                    chessBtn[curclicked_chessBtn.id]->hide();
                    chessBtn[curclicked_chessBtn.id]->alive = false;
                    haveChess[whole_loc_num[chessBtn[curclicked_chessBtn.id]->BtnLoc.x_loc][chessBtn[curclicked_chessBtn.id]->BtnLoc.y_loc]] = false;
                    needMove = true;
                    endGame(currentPlayer); // not necessarilly here (set a [check_endGame()] and call every step)
                }
                return true;
            }
        }
        else return false;
    }
    else if(preclicked_chessBtn.ID > curclicked_chessBtn.ID && curclicked_chessBtn.ID != 0){ // You > Other and Other not Mine
        // Other Die
        if(isReal){
            chessBtn[curclicked_chessBtn.id]->hide();
            chessBtn[curclicked_chessBtn.id]->alive = false;
            haveChess[whole_loc_num[chessBtn[curclicked_chessBtn.id]->BtnLoc.x_loc][chessBtn[curclicked_chessBtn.id]->BtnLoc.y_loc]] = false;
            needMove = true;
        }
        return true;
    }
    else return false;
}

// Make sure start() will only be called once (try to unable QAction startAct?)
void MainWindow::start(){

    // Moved to front
    ui->actionStart->setEnabled(false);
    ui->actionConnect_to_Server->setEnabled(false);
    ui->actionCreate_the_connection->setEnabled(false);

    ableStart_self = true; // not that much use

    if(!ableStart_else){
        // Only the 1st user who presses actionStart gets to enter here

        // Random Choose Firstplayer
        randNum = QRandomGenerator::global()->bounded(0, 2);
        messageSent = "actionStart_1" + QString::number(randNum);

        // Set to user1
        isUser1 = true;

        // sync randomArray here
        for(int i = 1; i <= 50; i++) randomArray[i] = i;
        for(int i = 1; i <= 50; i++){
            int swapPos = QRandomGenerator::global()->bounded(1, 51);
            std::swap(randomArray[i], randomArray[swapPos]);
        }
        for(int i = 1; i <= 50; i++){
            messageSent += int_to_char[randomArray[i]];
        }
        sendMessage();
    }

    if(ableStart_self && ableStart_else && !started){

        // Both enters here (after 2 users both presses actionStart)
        started = true;

        messageSent = "actionStart_2";
        sendMessage();

        // place chesspieces
        placeChesspieces();

        // displays user's turn info
        std::map<int, QString> user = {{1, user1}, {0, user2}};
        QString turnText = user[randNum];
        turnText += "'s Turn";
        ui->turnLb->setText(turnText);
        if(randNum == 1){
            currentPlayer.user = true;
            nextPlayer.user = false;
        }
        else{
            currentPlayer.user = false;
            nextPlayer.user = true;
        }

        // change "user info"
        QString userInfoText = user1;
        if(isUser1){
            userInfoText += " (You)\n";
            userInfoText += user2;
        }
        else{
            userInfoText += "\n";
            userInfoText += user2;
            userInfoText += " (You)";
        }
        ui->userInfoLb->setText(userInfoText);

        // Renew whether player can press
        if(isUser1 == currentPlayer.user){ // can press
            for(int i = 1; i <= 50; i++){
                chessBtn[i]->setEnabled(true);
                unknownBtn[i]->setEnabled(true);
            }
            for(int i = 1; i <= 60; i++)
                emptyBtn[i]->setEnabled(true);
        }
        else{ // can't press
            for(int i = 1; i <= 50; i++){
                chessBtn[i]->setEnabled(false);
                unknownBtn[i]->setEnabled(false);
            }
            for(int i = 1; i <= 60; i++)
                emptyBtn[i]->setEnabled(false);
        }

        // Set Countdown Timer
        ui->timerLCD->display(countDown);
        clock = new ChronoMarker(this);
        connect(clock, SIGNAL(freshSignal(int)), this, SLOT(show_clock(int)));

        // Change Round Label
        step++;
        round = (step + 1) / 2;
        QString roundText = "Round ";
        roundText += QString::number(round);
        ui->roundLb->setText(roundText);

        // be able to turn over the chesspieces on click
        for(int i = 1; i <= 50; i++){
            connect(unknownBtn[i], SIGNAL(clicked()), unknownBtn[i], SLOT(switch_turn_unmask()));
            connect(chessBtn[i], SIGNAL(clicked()), chessBtn[i], SLOT(press_own_chessBtn()));
            connect(chessBtn[i], SIGNAL(clicked()), chessBtn[i], SLOT(press_other_chessBtn()));

            // C/S connection
            connect(unknownBtn[i], SIGNAL(clicked()), unknownBtn[i], SLOT(switch_turn_unmask_message()));
            connect(chessBtn[i], SIGNAL(clicked()), chessBtn[i], SLOT(press_chessBtn_message()));

        }
        for(int i = 1; i <= 60; i++){
            connect(emptyBtn[i], SIGNAL(clicked()), emptyBtn[i], SLOT(press_emptyBtn()));
            connect(emptyBtn[i], SIGNAL(clicked()), emptyBtn[i], SLOT(press_emptyBtn_message())); // connection
        }
    }
    // Wait for other to start (Dialog Pop-out?)
}

void MainWindow::admit_defeat(){
    // Are you sure to admit defeat?
    InvalidRequest *w = new InvalidRequest(this);
    w->show();
    if(w->exec() == QDialog::Accepted){ // yes
        messageSent = "admit_defeat";
        if(isUser1 == currentPlayer.user){
            endGame(nextPlayer);
            messageSent += "next";
        }
        else{
            endGame(currentPlayer);
            messageSent += "cur";
        }
        show_clock(-101); // necessary, or else timer keeps going
        sendMessage();
    }
    else w->close(); // no
}

void MainWindow::createActions(){
    // Enable Menu
    ui->actionCreate_the_connection->setShortcuts(QKeySequence::New);
    ui->actionCreate_the_connection->setStatusTip(tr("Get Host IP to create the connection"));
    connect(ui->actionCreate_the_connection, &QAction::triggered, this, &MainWindow::open_server_panel);

    ui->actionConnect_to_Server->setShortcuts(QKeySequence::Open);
    ui->actionConnect_to_Server->setStatusTip(tr("Enter IP to connect to server"));
    connect(ui->actionConnect_to_Server, &QAction::triggered, this, &MainWindow::open_client_panel);

    ui->actionStart->setShortcuts(QKeySequence::Save);
    ui->actionStart->setStatusTip(tr("Start the game"));
    connect(ui->actionStart, &QAction::triggered, this, &MainWindow::start);
    ui->actionStart->setEnabled(false);

    ui->actionAdmit_the_defeat->setShortcuts(QKeySequence::Print);
    ui->actionAdmit_the_defeat->setStatusTip(tr("Admit the defeat and end the game"));
    connect(ui->actionAdmit_the_defeat, &QAction::triggered, this, &MainWindow::admit_defeat);
    ui->actionAdmit_the_defeat->setEnabled(false);

    ui->actionDisconnect->setShortcut(QKeySequence::Close);
    ui->actionDisconnect->setStatusTip(tr("Disconnect to end game immediately"));
    connect(ui->actionDisconnect, &QAction::triggered, this, &MainWindow::disconnect);
    ui->actionDisconnect->setEnabled(false);
}

void MainWindow::disconnect(){
    DisconnectNotice *w = new DisconnectNotice(this);
    if(w->exec() == QDialog::Rejected){
        checkState->stop();

        readWriteSocket->disconnectFromHost();

        end_game = true;
        show_clock(-101);
        QString showText = "---------";
        ui->turnLb->setText(showText);
        ui->timerLCD->display(88);

        // Renew Clicked Status
        for(int i = 1; i <= 50; i++){
            chessBtn[1]->deselect_chessBtn(i);
        }

        // Unable buttons and Menu
        for(int i = 1; i <= 50; i++){
            unknownBtn[i]->setEnabled(false);
            chessBtn[i]->setEnabled(false);
        }
        for(int i = 1; i <= 60; i++){
            emptyBtn[i]->setEnabled(false);
        }
        ui->actionAdmit_the_defeat->setEnabled(false);
    }
}

void MainWindow::generateLocation(){
    // 60 spaces
    int width = 58, height = 30;
    int x = 17, y = 82;
    for(int i = 1; i <= 6; i++){
        x = 17;
        for(int j = 1; j <= 5; j++){
            location[i][j] = new QRect(x, y, width, height);
            x += 80;
        }
        y += 40;
    }
    y = 403;
    for(int i = 7; i <= 12; i++){
        x = 17;
        for(int j = 1; j <= 5; j++){
            location[i][j] = new QRect(x, y, width, height);
            x += 80;
        }
        y += 40;
    }

    int i = 1;
    for(int y = 82; y <= 282; y = y + 40){ // 6 * i
        precise_estimate_y[y] = i;
        i++;
    }
    i = 7;
    for(int y = 403; y <= 603; y = y + 40){ // 6 * i
        precise_estimate_y[y] = i;
        i++;
    }
    int j = 1;
    for(int x = 17; x <= 337; x = x + 80){ // 5 * j
        precise_estimate_x[x] = j;
        j++;
    }
}

void MainWindow::init_mapLoc(){
    // pick 50 valid spaces
    int total = 0;
    for(int i = 1; i <= 12; i++){
        if(i == 3 || i == 5 || i == 8 || i == 10){
            for(int j = 1; j <= 5; j++){
                if(j == 1 || j == 3 || j == 5){
                    total++;
                    init_num_loc.insert(std::pair<int, QRect>(total, *location[i][j]));
                }
            }
        }
        else if(i == 4 || i == 9){
            for(int j = 1; j <= 5; j++){
                if(j != 3){
                    total++;
                    init_num_loc.insert(std::pair<int, QRect>(total, *location[i][j]));
                }
            }
        }
        else{
            for(int j = 1; j <= 5; j++){
                total++;
                init_num_loc.insert(std::pair<int, QRect>(total, *location[i][j]));
            }
        }
    }
}

void MainWindow::generateChesspieces(){

    // generate location & No. connected to location
    generateLocation();
    init_mapLoc();

    // generate empty chesspieces & place chesspieces
    int total = 0;
    for(int i = 1; i <= 12; i++){
        for(int j = 1; j <= 5; j++){
            total++;
            emptyBtn[total] = new ChessButton(this);
            emptyBtn[total]->setGeometry(*location[i][j]); // never move empty button
            emptyBtn[total]->BtnLoc = {i, j};
            emptyBtn[total]->setStyleSheet("ChessButton{border-image:url(:/other/Transparent.PNG);}");
            emptyBtn[total]->show();
            emptyBtn[total]->alive = true;
            emptyBtn[total]->id = total;
            emptyBtn[total]->isRed = -1;
            emptyBtn[total]->ID = -1;
        }
    }

    // generate chesspieces
    total = 0;
    for(int i = 0; i <= 11; i++){
        if(i <= 3){
            for(int j = 1; j <= 3; j++){
                total++;
                chessBtn[total] = new ChessButton(this);
                chessBtn[total]->initBtn(1, i, j, 3);
                chessBtn[25 + total] = new ChessButton(this);
                chessBtn[25 + total]->initBtn(0, i, j, 3);
            }
        }
        else if((i >= 4 && i <= 7) || i == 10){
            for(int j = 1; j <= 2; j++){
                total++;
                chessBtn[total] = new ChessButton(this);
                chessBtn[total]->initBtn(1, i, j, 2);
                chessBtn[25 + total] = new ChessButton(this);
                chessBtn[25 + total]->initBtn(0, i, j, 2);
            }
        }
        else{
            total++;
            chessBtn[total] = new ChessButton(this);
            chessBtn[total]->initBtn(1, i, 1, 1);
            chessBtn[25 + total] = new ChessButton(this);
            chessBtn[25 + total]->initBtn(0, i, 1, 1);
        }
    }

    // generate unknownBtns, and initial place chesspieces & unknownBtns (will change location after "start")
    for(int i = 1; i <= 50; i++){

        // place chesspieces as random order
        chessBtn[i]->setGeometry(init_num_loc[i]);
        chessBtn[i]->BtnLoc = {precise_estimate_y[(init_num_loc[i]).y()], precise_estimate_x[(init_num_loc[i]).x()]};
        chessBtn[i]->id = i;
        chessBtn[i]->show();
        chessBtn[i]->alive = true;

        // put unknown masks (question mark chesspieces) on top
        unknownBtn[i] = new UnknownButton(this);
        unknownBtn[i]->setGeometry(init_num_loc[i]);
        unknownBtn[i]->setStyleSheet("UnknownButton{border-image:url(:/other/unknown.png);}");
        unknownBtn[i]->id = i;
        unknownBtn[i]->show();
    }
}

void MainWindow::placeChesspieces(){ // place chesspieces randomly
    for(int i = 1; i <= 50; i++){
        // place chesspieces as random order
        chessBtn[i]->setGeometry(init_num_loc[randomArray[i]]);
        chessBtn[i]->BtnLoc = {precise_estimate_y[(init_num_loc[randomArray[i]]).y()], precise_estimate_x[(init_num_loc[randomArray[i]]).x()]};
        haveChess[whole_loc_num[chessBtn[i]->BtnLoc.x_loc][chessBtn[i]->BtnLoc.y_loc]] = true;

        // put unknown masks (question mark chesspieces) on top
        unknownBtn[i]->setGeometry(init_num_loc[randomArray[i]]);
        unknownBtn[i]->show();

        // Change Empty Space Alive Status to false
        emptyBtn[whole_loc_num[chessBtn[i]->BtnLoc.x_loc][chessBtn[i]->BtnLoc.y_loc]]->alive = false;
    }
}

void MainWindow::endGame(player winner){
    // Display winner info on MainWindow
    end_game = true;
    std::map<bool, QString> user = {{true, user1}, {false, user2}};
    QString showText = user[winner.user];
    showText += " wins!";
    ui->turnLb->setText(showText);
    ui->timerLCD->display(88);

    // Display winner info via dialog
    HaveWinner *w = new HaveWinner(this);
    w->show_text(showText);
    w->show();

    // Renew Clicked Status
    for(int i = 1; i <= 50; i++){
        chessBtn[1]->deselect_chessBtn(i);
    }

    // Unable buttons and Menu
    for(int i = 1; i <= 50; i++){
        unknownBtn[i]->setEnabled(false);
        chessBtn[i]->setEnabled(false);
    }
    for(int i = 1; i <= 60; i++){
        emptyBtn[i]->setEnabled(false);
    }
    ui->actionAdmit_the_defeat->setEnabled(false);

    checkState->stop();
}

bool MainWindow::noChessToMove(){
    // before new round starts, if [Next Player] has no chess to move, don't start new round

    if(unknown > 0) return false;

    // [Current Player] (Current player died with last chess) (or went into a dead end by him/herself, [no ors, other need to move])
    int ChessThatCanMove = 0; // Chess that "Can" Move
    for(int i = 1; i <= 50; i++){
        if(chessBtn[i]->isRed == currentPlayer.color && chessBtn[i]->alive && (chessBtn[i]->ID <= 10 && chessBtn[i]->ID >= 1)){ // check if nextPlayer's chess can move
            ChessThatCanMove++;
        }
    }
    if(ChessThatCanMove == 0){
        endGame(nextPlayer);
        return true;
    }

    // [Next Player]
    for(int i = 1; i <= 50; i++){
        if(chessBtn[i]->isRed == nextPlayer.color && chessBtn[i]->alive && (chessBtn[i]->ID <= 10 && chessBtn[i]->ID >= 1)){ // check if nextPlayer's chess can move
            preclicked_chessBtn = *chessBtn[i];
            for(int j = 1; j <= 50; j++){ // try every Other ChessAlive possible
                if(chessBtn[j]->alive && i != j && chessBtn[j]->isRed == currentPlayer.color){
                    curclicked_chessBtn = *chessBtn[j];
                    if(validPath() && validEat(false)){
                        return false;
                    }
                }
            }
            for(int j = 1; j <= 60; j++){ // try every EmptyAlive possible
                if(emptyBtn[j]->alive){
                    curclicked_chessBtn = *emptyBtn[j];
                    if(validPath()){
                        return false;
                    }
                }
            }
        }
    }
    endGame(currentPlayer);
    return true;
}

bool MainWindow::sbWins(){
    if(end_game == true) return true;
    else if(currentPlayer.exceedTime >= 3){ // Exceed Time * 3
        endGame(nextPlayer);
        return true;
    }
    else if(noChessToMove()) return true;
    else return false;
}

void MainWindow::show_clock(int s){

    // Set Countdown Timer + Renew Round + Renew Current Player
    if(s < 0){

        // Exceeded Time
        if(s > -10) currentPlayer.exceedTime++;

        // Check if any Winner
        if(sbWins()){
            clock->timer->stop();
            return;
        }

        // Renew Round
        step++;
        round = (step + 1) / 2;
        QString roundText = "Round ";
        roundText += QString::number(round);
        ui->roundLb->setText(roundText);

        // Check if can enable Admit Defeat Action
        if(step == 21) // THE 21st Step
            ui->actionAdmit_the_defeat->setEnabled(true);

        // Renew Current Player
        swapPlayers();
        std::map<bool, QString> user = {{true, user1}, {false, user2}};
        QString turnText = user[currentPlayer.user];
        turnText += "'s Turn";
        ui->turnLb->setText(turnText);

        // Renew whether player can press
        if(isUser1 == currentPlayer.user){ // can press
            for(int i = 1; i <= 50; i++){
                chessBtn[i]->setEnabled(true);
                unknownBtn[i]->setEnabled(true);
            }
            for(int i = 1; i <= 60; i++)
                emptyBtn[i]->setEnabled(true);
        }
        else{ // can't press
            for(int i = 1; i <= 50; i++){
                chessBtn[i]->setEnabled(false);
                unknownBtn[i]->setEnabled(false);
            }
            for(int i = 1; i <= 60; i++)
                emptyBtn[i]->setEnabled(false);
        }

        // Renew Clicked Status
        clicked_own_chessBtn = 0;
        clicked_other_chessBtn = 0;
        clicked_emptyBtn = 0;

        if(preclicked_chessBtn.id >= 1 && preclicked_chessBtn.id <= 50)
            chessBtn[1]->deselect_chessBtn(preclicked_chessBtn.id); // Deselect Own Chess
        if(curclicked_chessBtn.id >= 1 && curclicked_chessBtn.id <= 50)
            chessBtn[1]->deselect_chessBtn(curclicked_chessBtn.id); // Deselect Other Chess
        if(curclicked_chessBtn.id >= 1 && curclicked_chessBtn.id <= 60)
            emptyBtn[curclicked_chessBtn.id]->deselect_emptyBtn(); // Deselect Empty Chess

        for(int i = 1; i <= 50; i++){ // just in case
            chessBtn[1]->deselect_chessBtn(i);
        }

        preclicked_chessBtn.id = -1;
        curclicked_chessBtn.id = -1;

        // Set Timer
        clock->second = countDown;
        clock->timer->start();
        ui->timerLCD->display(countDown);
    }

    else ui->timerLCD->display(s); // Set Timer
}

MainWindow::~MainWindow(){
    delete ui;
}
