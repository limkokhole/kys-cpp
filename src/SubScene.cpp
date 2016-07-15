#include "SubScene.h"
#include "MainMap.h"
#include "Save.h"

SubScene::SubScene()
{
    full = 1;
}


SubScene::~SubScene()
{

}

void SubScene::draw()
{
    int k = 0;
    struct DrawInfo { int i; Point p; };
    std::map<int, DrawInfo> map;
    auto& m_SceneMapData = Save::getInstance()->m_SceneMapData;
    for (int sum = -sumregion; sum <= sumregion + 15; sum++)
    {
        for (int i = -widthregion; i <= widthregion; i++)
        {
            int i1 = Sx + i + (sum / 2);
            int i2 = Sy - i + (sum - sum / 2);
            auto p = getPositionOnScreen(i1, i2, Sx, Sy);
            if (i1 >= 0 && i1 <= MaxSceneCoord && i2 >= 0 && i2 <= MaxSceneCoord)
            {
                //EarthS[k]->setVisible(false);
                //BuildS[k]->setVisible(false);
                //����ע��״��
                Point p1 = Point(0, -m_SceneMapData[sceneNum].Data[4][i1][i2]);
                Point p2 = Point(0, -m_SceneMapData[sceneNum].Data[5][i1][i2]);
                int num = m_SceneMapData[sceneNum].Data[0][i1][i2] / 2;
                if (num >= 0)
                {
                    Texture::getInstance()->copyTexture("smap", num, p.x, p.y);
                    //                     if (p1.y < -0.1)
                    //                     {
                    //                         map[calBlockTurn(i1, i2, 0)] = s;
                    //                     }
                    //                     else
                    //                     {
                    //                         s->visit();
                    //                     }
                }
                //����������ͬһ��
                num = m_SceneMapData[sceneNum].Data[1][i1][i2] / 2;
                if (num > 0)
                {
                    Texture::getInstance()->copyTexture("smap", num, p.x, p.y);
                    map[calBlockTurn(i1, i2, 1)] = { num, p };
                }
                else if (i1 == Sx && i2 == Sy)
                {
                    manPicture = offset_manPic + Scene::towards * num_manPic + step;
                    map[calBlockTurn(i1, i2, 1)] = { manPicture, p };
                    //s->visit();
                }
                //�¼���
                num = m_SceneMapData[sceneNum].Data[3][i1][i2];
                //                 int picNum = m_SceneEventData[sceneNum].Data[num].BeginPic1 / 2;
                //                 if (num > 0 && m_SceneEventData[sceneNum].Data[num].IsActive >= 0 && picNum > 0)
                //                 {
                //                     auto t = MyTexture2D::getSelfPointer(MyTexture2D::Scene, picNum);
                //                     auto s = EventS[k];
                //                     t->setToSprite(s, p + p1, drawCount);
                //                     map[calBlockTurn(i1, i2, 2)] = s;
                //                 }
                num = m_SceneMapData[sceneNum].Data[2][i1][i2] / 2;
                if (num > 0)
                {
                    map[calBlockTurn(i1, i2, 3)] = { num, p };
                }
            }
            k++;
        }
    }
    for (auto i = map.begin(); i != map.end(); i++)
    {
        Texture::getInstance()->copyTexture("smap", i->second.i, i->second.p.x, i->second.p.y);
    }

}

void SubScene::init()
{
    Save::getInstance()->LoadR(0);
    Sx = 32;
    Sy = 32;
    //Sx = m_SceneData[sceneNum].EntranceX;
    //Sy = m_SceneData[sceneNum].EntranceY;
}

void SubScene::dealEvent(BP_Event& e)
{
    int x = Sx, y = Sy;
    if (!wayQue.empty())
    {
        //             MyPoint newMyPoint = wayQue.top();
        //             x = newMyPoint.x;
        //             y = newMyPoint.y;
        //             checkIsExit(x, y);
        //             Towards myTowards = (Towards)(newMyPoint.towards);
        //             log("myTowards=%d", myTowards);
        //             Walk(x, y, myTowards);
        //             wayQue.pop();
    }
    if (e.type == BP_KEYDOWN)
    {
        switch (e.key.keysym.sym)
        {
        case BPK_LEFT:
        {
            y--;
            checkIsExit(x, y);
            Walk(x, y, LeftDown);
            stopFindWay();
            break;
        }
        case BPK_RIGHT:
        {
            y++;
            checkIsExit(x, y);
            Walk(x, y, RightUp);
            stopFindWay();
            break;
        }
        case BPK_UP:
        {
            x--;
            checkIsExit(x, y);
            Walk(x, y, LeftUp);
            stopFindWay();
            break;
        }
        case BPK_DOWN:
        {
            x++;
            checkIsExit(x, y);
            Walk(x, y, RightDown);
            stopFindWay();
            break;
        }
        case BPK_ESCAPE:
        {
            stopFindWay();
            break;
        }
        case BPK_RETURN:
        case BPK_SPACE:
        {
            stopFindWay();
            callEvent(x, y);
            break;
        }
        default:
        {
        }
        }
    }
}

void SubScene::Walk(int x, int y, Towards t)
{
    if (canWalk(x, y))
    {
        Sx = x;
        Sy = y;
    }
    if (Scene::towards != t)
    {
        Scene::towards = t;
        step = 0;
    }
    else
    {
        step++;
    }
    step = step % num_manPic;
}

bool SubScene::canWalk(int x, int y)
{
    if (checkIsOutLine(x, y) || checkIsBuilding(x, y) || checkIsHinder(x, y)
        || checkIsEvent(x, y) || checkIsFall(x, y))
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool SubScene::checkIsBuilding(int x, int y)
{
    //     if (m_SceneMapData[sceneNum].Data[1][x][y] >= -2 && m_SceneMapData[sceneNum].Data[1][x][y] <= 0)
    //     {
    //         return false;
    //     }
    //     else
    {
        return true;
    }
}

bool SubScene::checkIsOutLine(int x, int y)
{
    if (x < 0 || x > MaxSceneCoord || y < 0 || y > MaxSceneCoord)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool SubScene::checkIsHinder(int x, int y)
{
    //     if (m_SceneMapData[sceneNum].Data[0][x][y] >= 358 && m_SceneMapData[sceneNum].Data[0][x][y] <= 362
    //         || m_SceneMapData[sceneNum].Data[0][x][y] == 522 || m_SceneMapData[sceneNum].Data[0][x][y] == 1022
    //         || m_SceneMapData[sceneNum].Data[0][x][y] >= 1324 && m_SceneMapData[sceneNum].Data[0][x][y] <= 1330
    //         || m_SceneMapData[sceneNum].Data[0][x][y] == 1348)
    //     {
    //         return true;
    //     }
    return false;
}

bool SubScene::checkIsEvent(int x, int y)
{
    //     //if (save.SData[sceneNum].SData[4][x][y] >= 0 && (save.DData[sceneNum].DData[save.SData[sceneNum].SData[3][x][y],0] % 10)<1)
    //     int num = m_SceneMapData[sceneNum].Data[3][x][y];
    //     int canWalk = m_SceneEventData[sceneNum].Data[num].CanWalk;
    //     if (canWalk > 0)
    //     {
    //         return true;
    //     }
    return false;
}

bool SubScene::checkIsFall(int x, int y)
{
    //     if (abs(m_SceneMapData[sceneNum].Data[4][x][y] - m_SceneMapData[sceneNum].Data[4][Sx][Sy] > 10))
    //     {
    //         true;
    //     }
    return false;
}

bool SubScene::checkIsExit(int x, int y)
{
    //     if ((int)m_SceneData[sceneNum].ExitX[0] == x && (int)m_SceneData[sceneNum].ExitY[0] == y
    //         || (int)m_SceneData[sceneNum].ExitX[2] == x && (int)m_SceneData[sceneNum].ExitY[2] == y)
    //     {
    //         auto map = MainMap::createScene();
    //         auto transitionMap = TransitionPageTurn::create(0.2f, map, false);
    //         this->pause();
    //         Director::getInstance()->replaceScene(transitionMap);
    //         return true;
    //     }
    //     else if ((int)m_SceneData[sceneNum].ExitX[1] == x && (int)m_SceneData[sceneNum].ExitY[1] == y)
    //     {
    //         /*
    //         SaveGame::getInstance()->RBasic_Data.Mface = towards;
    //         SaveGame::getInstance()->RBasic_Data.Mx = save.RScene[sceneNum]->MainEntranceX2;
    //         SaveGame::getInstance()->RBasic_Data.My = save.RScene[sceneNum]->MainEntranceY2;
    //         Mx = save.RScene[sceneNum]->MainEntranceX2;
    //         My = save.RScene[sceneNum]->MainEntranceY2;
    //         MainMap* mainMap = dynamic_cast<MainMap*>(Director::getInstance()->getRunningScene()->getChildByTag(MainMap::tag_mainLayer));
    //         CommonScene::replaceLocation();
    //         */
    //         auto map = MainMap::createScene();
    //         auto transitionMap = TransitionPageTurn::create(0.2f, map, false);
    //         this->pause();
    //         Director::getInstance()->replaceScene(transitionMap);
    //         return true;
    //     }
    return false;
}

void SubScene::callEvent(int x, int y)
{
    if (checkIsEvent(x, y))
    {
        //         int num = m_SceneMapData[sceneNum].Data[3][x][y];
        //         int eventNum = m_SceneEventData[sceneNum].Data[num].Num;
                //�������ΪeventNum���¼�
    }
}

bool SubScene::checkIsOutScreen(int x, int y)
{
    if (abs(Sx - x) >= 2 * widthregion || abs(Sy - y) >= sumregion)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void SubScene::getMousePosition(Point *point)
{
    //     int x = point->x;
    //     int y = Center_Y * 2 - point->y;
    //     //int yp = 0;
    //     int yp = -(m_SceneMapData[sceneNum].Data[4][x][y]);
    //     Msx = (-x + Center_X + 2 * (y + yp) - 2 * Center_Y + 18) / 36 + Sx;
    //     Msy = (x - Center_X + 2 * (y + yp) - 2 * Center_Y + 18) / 36 + Sy;
}

void SubScene::FindWay(int Mx, int My, int Fx, int Fy)
{
    //     bool visited[479][479] = { false };									//�ѷ��ʱ��(�ر��б�)
    //     int dirs[4][2] = { { -1, 0 }, { 0, 1 }, { 0, -1 }, { 1, 0 } };		//�ĸ�����
    //     MyPoint *myPoint = new MyPoint;
    //     myPoint->x = Mx;
    //     myPoint->y = My;
    //     myPoint->towards = (MyPoint::Towards)CallFace(Mx, My, Fx, Fy);
    //     myPoint->parent = myPoint;
    //     myPoint->Heuristic(Fx, Fy);
    //     //log("Fx=%d,Fy=%d", Fx, Fy);
    //     //log("Mx=%d,My=%d", Mx, My);
    //     while (!wayQue.empty())
    //     {
    //         wayQue.pop();
    //     }
    //     priority_queue<MyPoint*, vector<MyPoint*>, Compare> que;			//��С���ȼ�����(�����б�)
    //     que.push(myPoint);
    //     int sNum = 0;
    //     while (!que.empty() && sNum <= 300)
    //     {
    //         MyPoint *t = new MyPoint();
    //         t = que.top();
    //         que.pop();
    //         visited[t->x][t->y] = true;
    //         sNum++;
    //         //log("t.x=%d,t.y=%d",t->x,t->y);
    //         if (t->x == Fx && t->y == Fy)
    //         {
    //             minStep = t->step;
    //             wayQue.push(*t);
    //             int k = 0;
    //             while (t != myPoint&&k <= minStep)
    //             {
    //                 //log("t.x=%d,t.y=%d,s.x=%d,s.y=%d,t.f=%d", t->x, t->y, t->parent->x, t->parent->y,t->f);
    // 
    //                 t->towards = t->parent->towards;
    //                 wayQue.push(*t);
    //                 t = t->parent;
    //                 k++;
    //                 //log("go in!");
    //             }
    //             //log("minStep=%d", minStep);
    //             //log("wayQue=%d", wayQue.size());
    //             break;
    //         }
    //         else
    //         {
    //             for (int i = 0; i < 4; i++)
    //             {
    //                 MyPoint *s = new MyPoint();
    //                 s->x = t->x + dirs[i][0];
    //                 s->y = t->y + dirs[i][1];
    //                 if (canWalk(s->x, s->y) && !checkIsOutScreen(s->x, s->y) && !visited[s->x][s->y])
    //                 {
    //                     s->g = t->g + 10;
    //                     s->towards = (MyPoint::Towards)i;
    //                     if (s->towards == t->towards)
    //                     {
    //                         s->Heuristic(Fx, Fy);
    //                     }
    //                     else
    //                     {
    //                         s->h = s->Heuristic(Fx, Fy) + 1;
    //                     }
    //                     s->step = t->step + 1;
    //                     s->f = s->g + s->h;
    //                     //t->towards = (MyPoint::Towards)i;
    //                     //s->Gx = dirs[i][0];
    //                     //s->Gy = dirs[i][1];
    //                     //t->child[i] = s;
    //                     //if (s->parent)
    //                     s->parent = t;
    //                     //log("s.x=%d,s.y=%d,t.x=%d,t.y=%d", s->x, s->y, t->x, t->y);
    //                     //log("s.g=%d,s.h=%d,s.f=%d", s.g, s.h, s.f);
    //                     que.push(s);
    //                 }
    //             }
    //         }
    //     }
    //     myPoint->delTree(myPoint);
}

void SubScene::stopFindWay()
{
    while (!wayQue.empty())
    {
        wayQue.pop();
    }
}