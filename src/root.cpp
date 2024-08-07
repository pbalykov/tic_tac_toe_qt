/****************************************************************************
 **                                                                        **
 **                           TicTacToeQt GUI игра                         **
 **                                                                        **
 **  Эта программа является свободным программным обеспечением: вы можете  **
 **  распространять ее и/или модифицировать в соответствии с условиями     **
 **  GNU General Public License, опубликованной Фондом свободного          **
 **  программного обеспечения, либо версии 3 Лицензии, либо                **
 **  (по вашему выбору) любой более поздней версии.                        **
 **                                                                        **
 **  Эта программа распространяется в надежде, что она будет полезна,      **
 **  но БЕЗ КАКИХ-ЛИБО ГАРАНТИЙ; даже без подразумеваемых гарантий         **
 **  ТОВАРНАЯ ПРИГОДНОСТЬ или ПРИГОДНОСТЬ ДЛЯ ОПРЕДЕЛЕННОЙ ЦЕЛИ. Увидеть   **
 **  GNU General Public License для получения более подробной информации.  **
 **                                                                        **
 **  Вы должны были получить копию GNU General Public License вместе с     **
 **  этой программой.  Если нет, смотрите http://www.gnu.org/licenses/.    **
 **                                                                        **
 ****************************************************************************
 **          Author: pbalykov                                              **
 ** Website/Contact: https://github.com/pbalykov                           **
 **            Date: 09.07.24                                              **  
 **         Version: 1.0.0                                                 **
 ****************************************************************************/

#include "../include/root.hpp"
#include "../include/descriptions.hpp"

#include "ui_root.h"

#include <QActionGroup>
#ifdef QT_DEBUG
#include <QDebug>
#endif


Root::Root(QWidget* parent) : QMainWindow(parent), _ui(new Ui::Root),
    _complexity_game(TicTacToe::COMPLEXITY::AVERAGE), _isTwoPlayers(false),
    _first_move(TicTacToe::CELL_VALUE::CROSS), _game(_complexity_game, _first_move)
{

    this->_ui->setupUi(this);
    this->setWindowTitle(NAME_WINDOW);

    this->_ui->gameStatus->hide();

    this->_button_cell = QVector<Cell*>(
                {this->_ui->cell1, this->_ui->cell2, this->_ui->cell3,
                 this->_ui->cell4, this->_ui->cell5, this->_ui->cell6,
                 this->_ui->cell7, this->_ui->cell8, this->_ui->cell9}
                );

    int caunt = 0;
    for (auto i : this->_button_cell) {
        auto f_slot = [caunt, this] () { this->_buttonPress(caunt); };
        QObject::connect(i, &Cell::clicked, f_slot);
        caunt++;
    }
    this->_create_group();
}

Root::~Root() {
    delete this->group1;
    delete this->group2;
    delete this->_ui;
}

void Root::_create_group() {
    this->group1 = new QActionGroup(this);
    this->group1->addAction(this->_ui->complexity_1);
    this->group1->addAction(this->_ui->complexity_2);
    this->group1->addAction(this->_ui->complexity_3);
    this->group1->setExclusive(true);

    this->group2 = new QActionGroup(this);
    this->group2->addAction(this->_ui->first_player);
    this->group2->addAction(this->_ui->second_player);
    this->group2->setExclusive(true);
}

void Root::_endGame() {
    if ( this->_game.getEndGame() ) {
        this->_ui->gameStatus->show();
        int value = 0;
        switch ( this->_game.getWing() ) {
            case TicTacToe::CELL_VALUE::ZERO :
                value = this->_ui->lcdZero->value();
                this->_ui->lcdZero->display(++value);
                this->_ui->gameStatus->setStyleSheet(Cell::CSS_ZERO);
                this->_ui->gameStatus->setText(WING_ZERO);
                break;
            case TicTacToe::CELL_VALUE::CROSS :
                value = this->_ui->lcdCross->value();
                this->_ui->lcdCross->display(++value);
                this->_ui->gameStatus->setStyleSheet(Cell::CSS_CROSS);
                this->_ui->gameStatus->setText(WING_CROSS);
                break;
            default:
                this->_ui->gameStatus->setStyleSheet(Cell::CSS_DEFAULT);
                this->_ui->gameStatus->setText(DRAW);
                break;
        }
    }
}

void Root::_buttonPress(int index) {
    if ( this->_game.getEndGame() )
        return ;
    auto type_cell = this->_game.setValue(index);
    this->_installCell(index, type_cell);
    if ( !this->_isTwoPlayers ) {
        auto valueBot = this->_game.setBot();
        this->_installCell(valueBot.second, valueBot.first);
    }
    this->_endGame();
}

void Root::_installCell(int index, TicTacToe::CELL_VALUE type_cell) {
    switch ( type_cell ) {
        case TicTacToe::CELL_VALUE::ZERO :
            this->_button_cell[index]->setCell(TYPE_CELL::ZERO);
            break;
        case TicTacToe::CELL_VALUE::CROSS :
            this->_button_cell[index]->setCell(TYPE_CELL::CROSS);
            break;
        default :
            break;
    }
}

void Root::on_updateButton_clicked() {
    this->_ui->gameStatus->hide();
    for (auto i : this->_button_cell) {
        i->clear();
    }
    this->_first_move = this->_first_move == TicTacToe::CELL_VALUE::CROSS ?
                TicTacToe::CELL_VALUE::ZERO : TicTacToe::CELL_VALUE::CROSS;
    this->_game = TicTacToe(this->_complexity_game, this->_first_move);
    if ( this->_first_move == TicTacToe::CELL_VALUE::ZERO && !this->_isTwoPlayers ) {
        auto valueBot = this->_game.setBot();
        this->_installCell(valueBot.second, valueBot.first);
    }
}

void Root::on_action_exit_triggered() {
    this->close();
}

void Root::on_new_game_triggered() {
    this->_ui->lcdZero->display(0);
    this->_ui->lcdCross->display(0);
    _first_move = TicTacToe::CELL_VALUE::ZERO;
    this->on_updateButton_clicked();
}

void Root::on_call_help_triggered() {
    Descriptions* secondWindow = new Descriptions(this);
    secondWindow->exec();
}

void Root::on_complexity_1_triggered() {
    this->_complexity_game = TicTacToe::COMPLEXITY::LIGHT;
    this->on_new_game_triggered();
}

void Root::on_complexity_2_triggered() {
    this->_complexity_game = TicTacToe::COMPLEXITY::AVERAGE;
    this->on_new_game_triggered();
}

void Root::on_complexity_3_triggered() {
    this->_complexity_game = TicTacToe::COMPLEXITY::COMPLEX;
    this->on_new_game_triggered();
}

void Root::on_first_player_triggered() {
    if ( !this->_isTwoPlayers ) {
        return ;
    }
    this->_isTwoPlayers = false;
    this->_ui->menu_complexity->setEnabled(true);
    this->on_new_game_triggered();
}

void Root::on_second_player_triggered() {
    if ( this->_isTwoPlayers ) {
        return ;
    }
    this->_isTwoPlayers = true;
    this->_ui->menu_complexity->setEnabled(false);
    this->on_new_game_triggered();
}
