#pragma once
#include "jsonbridge.h"
//#include "myfont.h"

constexpr char* vertt = "#version 330 core \nlayout(location = 0) in vec3 pos; \nlayout(location = 1) in vec3 color; \nuniform mat4 MVP; \nuniform vec3 COL[15000]; \nout vec3 vert_color; \nvoid main() { \nvert_color = color; \ngl_Position = MVP * vec4(pos, 1.0); \n}";
constexpr char* fragg = "#version 330 core \nin vec3 vert_color; \nout vec4 FragColor; \nvoid main() { \nFragColor = vec4(vert_color, 1.0); \n}";

// meaningless initialization except BUTTONY and DIPLAY
int DISPLAYX = 0; // exception
int DISPLAYY = 0; // exception
int SCREENX = 2560;
int SCREENY = 1440;
int BUTTONX = 225;
// const int BUTTONY = 90; // exception
int MENUX = 370;
int MENUY = 1440;
int CANVASX = 2305;
int CANVASY = 1193;
int INDEXX = 2305;
int INDEXY = 247;
float SCREENSCALE = 2.305f;
float CXDY = 2.0; // canvas x div y, only calculated once, but not here
int REALY = 500; // also calculated once, also not here
float scale = 23.57f;
//
Area empty;
ImU32 colors[LEVELCALCULATEDMAX];
ImU32 bcolor[LEVELCALCULATEDMAX];
float px = 1.0f; // Repixel()
float pz = 0.0f; // ClickorDrag()
float zy = 1.5f; // Repixel()
ImVec2 mouse(0, 0);
ImVec2 mousehist(0, 0);
ImVec2 seat(0, 0);
bool bordersetting = false;
bool barriersetting = false;
bool startsetting = false;
bool endsetting = false;
Area* area = nullptr;
std::vector<Area> arealist;
Border border, newborder, newbarrier;
std::vector<Border> barrierlist;
int barrierkk = 0;
unsigned char pixels[XMAX * YMAX][4];
bool press = false;
bool presshist = false;
float playtime = 0.0f;
bool playing = false;
bool dynamik = false;
float playspeed = 1.0f;
float timeup = 600.0f;
float deltatime = 1000.0f;
float tank = 0.0f;
int fpshist = 1;
float PUTUP = 0.0f;
int PUTUPLITTLE = 0;
ImVec2 bstart(0, 0);
ImVec2 bend(0, 0);
float endist = 5.0;
float endeflex = 0.0f;
int levelvirtualnum = 0;
int drawmode = 0;
int linewidth = 5;
int sidewidth = 10;
std::vector<Playtime> schedulist;
std::vector<Person> peoplist;
std::vector<std::vector<bool>> crush;
std::vector<int> occupeople;
std::vector<int> levell;

std::atomic<bool> signal(false);
std::atomic<float> makepercent(0.0f);
std::atomic<bool> makefinish(false);

std::mt19937 gen(std::random_device{}());

int main(int argc, char **argv) 
{
    std::uniform_int_distribution<> dis(128, 255);
    for (int i = 0; i < LEVELCALCULATEDMAX; i++)
    {
        int vace = 0, rr, rg, rb;
        rr = rg = rb = 0;
        while (vace < 5000)
        {
            rr = dis(gen);
            rg = dis(gen);
            rb = dis(gen);
            vace = (rr - rg) * (rr - rg) + (rr - rb) * (rr - rb) + (rg - rb) * (rg - rb);
        }
        colors[i] = IM_COL32(rr, rg, rb, 255);
    }
    glfwInit();
    int modcnt = 0;
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoModes(monitor, &modcnt);
    for (int i = 0; i < modcnt; i++)
    {
        if (DISPLAYX < mode[i].width) DISPLAYX = mode[i].width;
        if (DISPLAYY < mode[i].height) DISPLAYY = mode[i].height;
    }
    BUTTONX = BUTTONMIN;
    INDEXY = INDEXMIN;
    MENUX = BUTTONX * BUTTON2MENU;
    CXDY = 1.0f * (DISPLAYX - BUTTONMIN) / (DISPLAYY - INDEXMIN);
    CANVASX = REALX;
    INDEXX = CANVASX;
    SCREENX = CANVASX + BUTTONX + 15;
    CANVASY = CANVASX / CXDY;
    SCREENY = CANVASY + INDEXY;
    MENUY = SCREENY;
    REALY = static_cast<int>(roundf(REALX / CXDY));
    SCREENSCALE = CANVASX / REALX;
    InitializeBorder();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window = glfwCreateWindow(SCREENX, SCREENY, "Milk", nullptr, nullptr);
    glfwSetWindowSizeLimits(window, 4 * BUTTONMIN, 3 * INDEXMIN, DISPLAYX, DISPLAYY);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = NULL;
    //io.Fonts->AddFontFromMemoryCompressedTTF(Msyhbd_compressed_data, Msyhbd_compressed_size, 48.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());
    static_cast<void>(io);
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.FramePadding = ImVec2(36, 36);
    style.ItemInnerSpacing = ImVec2(5, 6);
    style.ItemSpacing = ImVec2(5, 6);
    style.SeparatorTextPadding = ImVec2(36, 0);
    style.SeparatorTextBorderSize = 15;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    Eigen::Matrix4f view_mat = get_view_matrix(Eigen::Vector3f(0.0f, 0.0f, 1.5f));
    Eigen::Matrix4f proj_mat = get_projection_matrix(120.0f, 1.0f, 0.1f, 10.0f);
    Eigen::Matrix4f mvp_mat = proj_mat * view_mat;
    Shader shader(vertt, fragg);
    shader.use_program();
    uint32_t view_loc = glGetUniformLocation(shader.get_id(), "MVP");
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, mvp_mat.data());
    area = &empty;
    char buf[128] = "";
    char areafilename[128] = "areafilename";
    char scenefilename[128] = "scenefilename";
    char playfilename[128] = "playfilename";
    std::vector<std::thread> pool;
    int thisareanum = arealist.size();
    while (!glfwWindowShouldClose(window))
    {
        auto start = std::chrono::high_resolution_clock::now();
        process_input(window);
        glfwPollEvents();
        ClickorDrag();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        bool thisignal = signal;
        if (!thisignal) thisareanum = arealist.size();
        float thispercent = makepercent;
        bool thisplaying = playing;
        ImGui::Begin("Index", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
        ImGui::SetWindowSize("Index", ImVec2(INDEXX, INDEXY));
        ImGui::SetWindowPos("Index", ImVec2(0, CANVASY));
        ImGui::SetWindowFontScale(0.5f);
        style.FrameRounding = 0.0f;
        ImVec2 INDEX1(INDEXX / 4.0f - 7.8f, INDEXY / 2.0f - 41.0f);
        ImVec2 INDEX2(INDEXX / 2.0f - 10.6f, INDEXY / 2.0f - 41.0f);
        ImGui::BeginDisabled();
        ImGui::SeparatorText(" 指标 ");
        sprintf(buf, " 坪效 : %.2f ", px);
        ImGui::Button(buf, INDEX1); ImGui::SameLine();
        sprintf(buf, " 增益 : %.2f ", zy);
        ImGui::Button(buf, INDEX1); ImGui::SameLine();
        sprintf(buf, " 平均偏转角度 : %.2f ", pz);
        ImGui::Button(buf, INDEX2);
        ImGui::SeparatorText(" 信息 ");
        sprintf(buf, " 帧率 : %d ", fpshist);
        ImGui::Button(buf, INDEX1); ImGui::SameLine();
        float posx, posy;
        if (area->kk == 0)
        {
            posx = mouse.x;
            posy = REALY - mouse.y;
        }
        else
        {
            posx = area->pos.x;
            posy = REALY - area->pos.y;
        }
        if (posx < 0) posx = 0;
        if (posx > REALX) posx = REALX;
        if (posy < 0) posy = 0;
        if (posy > REALY) posy = REALY;
        sprintf(buf, " (%.1f,%.1f) ", posx / scale, posy / scale);
        ImGui::Button(buf, INDEX1); ImGui::SameLine();
        tank += deltatime;
        if (tank > 0.2f)
        {
            tank -= 0.2f;
            fpshist = static_cast<int>(1.0f / deltatime);
            if (fpshist < 0 || fpshist>1000) fpshist = 1000;
        }
        int levelrealnum = 0;
        if (thisignal) sprintf(buf, " %.0f%% ", 100.0f * thispercent);
        else
        {
            for (int i = 0; i < arealist.size(); i++)
                if (arealist[i].ishead) levelrealnum++;
            sprintf(buf, " 关卡数量 : %d/%d ", levelrealnum, levelvirtualnum);
        }
        ImGui::Button(buf, INDEX2);
        ImGui::EndDisabled();
        ImGui::End();

        ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
        ImGui::SetWindowSize("Menu", ImVec2(MENUX, MENUY));
        ImGui::SetWindowPos("Menu", ImVec2(CANVASX, 0));
        ImGui::SetWindowFontScale(0.5f);
        thisignal = signal;
        if (thisignal) ImGui::BeginDisabled();
        style.FrameRounding = 12.0f;
        ImVec2 BUTTON(BUTTONX, BUTTONY);
        if (playing) ImGui::BeginDisabled();
        int tempacreage = border.acreage;
        ImVec2 tempsize = area->size;
        float temprot = area->rot;
        ImGui::SeparatorText(" 场地 ");
        if (ImGui::Button(" 新建场地 ", BUTTON)) SetBorder();
        if (ImGui::DragInt("  1 ", &(border.acreage), 0.1f, 100, 9600000, " 实际面积 : %d "))
        {
            scale = sqrt(1.0 * border.screenAcreage / border.acreage);
            bool flag = true;
            for (int i = 0; i < arealist.size(); i++)
            {
                if (!IsAinB(arealist[i], 1))
                {
                    border.acreage = tempacreage;
                    scale = sqrt(1.0 * border.screenAcreage / border.acreage);
                    flag = false;
                    break;
                }
            }
            if (flag) Refind();
        }
        if (ImGui::Button(" 更新场地 ", BUTTON)) BuildBorder();
        if (ImGui::Button(" 更改起点 ", BUTTON)) SetStart();
        if (ImGui::Button(" 更改终点 ", BUTTON)) SetEnd();
        ImGui::DragFloat("  2 ", &endeflex, 0.1f, 0.0f, 360.0f, " 偏转角度 : %.1f ");
        ImGui::SetItemTooltip(" 从水平向右起 逆时针转 ");
        ImGui::InputText("  3 ", scenefilename, 100);
        std::string sfnstr(scenefilename);
        if (ImGui::Button(" 导入场地 ", BUTTON)) ImportScene(sfnstr);
        if (ImGui::Button(" 导出场地 ", BUTTON)) ExportScene(sfnstr);
        ImGui::SeparatorText(" 障碍 ");
        if (ImGui::Button(" 新建障碍 ", BUTTON)) SetBarrier();
        if (ImGui::Button(" 更新障碍 ", BUTTON)) BuildBarrier();
        int thisbarriermax = barrierlist.size();
        if (thisbarriermax == 0) ImGui::BeginDisabled();
        ImGui::DragInt("  4 ", &barrierkk, 0.01f, 0, thisbarriermax, " 障碍 : %d ");
        if (ImGui::Button(" 删除障碍 ", BUTTON)) DeleteBarrier();
        if (thisbarriermax == 0) ImGui::EndDisabled();
        ImGui::SeparatorText(" 关卡 ");
        if (ImGui::Button(" 新建关卡 ", BUTTON)) AddArea();
        ImGui::InputText("  5 ", areafilename, 100);
        std::string afnstr(areafilename);
        if (ImGui::Button(" 导入关卡 ", BUTTON))
        {
            area = &empty;
            while (arealist.size() > 0)
                arealist.pop_back();
            thisareanum = 0;
            makepercent = 0.0f;
            signal = true;
            pool.emplace_back(ImportArea, afnstr);
        }
        if (ImGui::Button(" 导出关卡 ", BUTTON)) ExportArea(afnstr);
        sprintf(buf, " 关卡 %d ", area->kk);
        ImGui::SeparatorText(buf);
        int thiskk = area->kk;
        if (thiskk == 0) ImGui::BeginDisabled();
        if (ImGui::Button(" 删除关卡 ", BUTTON)) DeleteArea();
        ImGui::DragFloat("  6 ", &(area->size.x), 0.1f, 1, 3000, " 长度 : %.1f ");
        ImGui::DragFloat("  7 ", &(area->size.y), 0.1f, 1, 3000, " 宽度 : %.1f ");
        ImGui::DragInt("  8 ", &(area->tshow), 0.1f, 0, 31536000, " 始于 : %ds ");
        ImGui::DragInt("  9 ", &(area->tlast), 0.1f, 1, 31536000, " 持续 : %ds ");
        if (thiskk == 1) ImGui::BeginDisabled();
        if (ImGui::Checkbox(" 关卡头部 ", &(area->ishead))) ReCntLevels();
        if (thiskk == 1) ImGui::EndDisabled();
        if (ImGui::Checkbox(" 固定此时方向 ", &(area->dirsolid)))
        {
            if (area->dirsolid && area->kk >= 2)
            {
                int i = area->kk - 1;
                ImVec2 dirc = arealist[i].pos - arealist[i - 1].pos;
                if (Dot(dirc, dirc) > EPS) area->dirc = ~dirc;
                else area->dirc = ImVec2(1, 0);
            }
        }
        ImGui::DragFloat("  10 ", &(area->rot), 0.1f, 0.0f, 360.0f, " 自旋角度 : %.1f ");
        ImGui::SetItemTooltip(" 从水平向右起 逆时针转 ");
        ImGui::DragFloat("  11 ", &(area->deflex), 0.1f, 0.0f, 360.0f, " 偏转角度 : %.1f ");
        ImGui::SetItemTooltip(" 从水平向右起 逆时针转 ");
        if (area->kk > 0)
        {
            if (!IsAinB(*area, 1))
            {
                area->size = tempsize;
                area->rot = temprot;
            }
        }
        if (ImGui::Button(" 对齐偏转方向 ", BUTTON)) SeefalseHoldtrue(false);
        if (ImGui::Button(" 贴紧前一关卡 ", BUTTON)) SeefalseHoldtrue(true);
        if (thiskk == 0) ImGui::EndDisabled();
        if (thisplaying) ImGui::EndDisabled();
        ImGui::SeparatorText(" 操作 ");
        int thisdm = drawmode;
        if (thisplaying) ImGui::BeginDisabled();
        if (thisdm == 1 || thisdm == 2) ImGui::BeginDisabled();
        sprintf(buf, "%.2fs", timeup - playtime);
        if (thisplaying && (!dynamik)) ImGui::Button(buf, BUTTON);
        else if (ImGui::Button(" 开始！ ", BUTTON)) PlayNow();
        ImGui::InputText("  12 ", playfilename, 100);
        std::string pfnstr(playfilename);
        if (thisplaying && dynamik) ImGui::Button(buf, BUTTON);
        else if (ImGui::Button(" 导入游客 ", BUTTON)) PlayDynamic(pfnstr);
        if (thisdm == 1 || thisdm == 2) ImGui::EndDisabled();
        ImGui::DragFloat("  13 ", &timeup, 0.01f, 1, 31536000, " 时长 : %.1fs ");
        if (thisplaying) ImGui::EndDisabled();
        ImGui::DragFloat("  14 ", &playspeed, 0.01f, 0.001, 1000, " 速度 : x%.1f ");
        if (thisplaying) ImGui::BeginDisabled();
        if (thisareanum != 0) ImGui::BeginDisabled();
        if (ImGui::DragInt("  15 ", &PUTUPLITTLE, 0.01f, 0, 30000, " 容忍误差 : %d "))
        {
            PUTUP = PUTUPLITTLE * 64;
            Refind();
        }
        if (thisareanum != 0) ImGui::EndDisabled();
        ImGui::DragInt("  16 ", &drawmode, 0.02f, 0, 3, " 绘制模式 : %d ");
        thisdm = drawmode;
        if (thisdm != 1 && thisdm != 3) ImGui::BeginDisabled();
        ImGui::DragInt("  17 ", &sidewidth, 0.1f, 1, 30000, " 框线宽度 : %d ");
        if (thisdm != 1 && thisdm != 3) ImGui::EndDisabled();
        ImGui::DragInt("  18 ", &linewidth, 0.1f, 1, 30000, " 通道宽度 : %d ");
        if (thisplaying) ImGui::EndDisabled();
        if (thisignal) ImGui::EndDisabled();
        ImGui::End();

        ImGui::Begin("Canvas", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
        ImGui::SetWindowSize("Canvas", ImVec2(CANVASX, CANVASY));
        ImGui::SetWindowPos("Canvas", ImVec2(0, 0));
        ImGui::SetWindowFontScale(SCREENSCALE);
        ImDrawList* drawlist = ImGui::GetWindowDrawList();
        thisignal = signal;
        ///////////////////////////
        /////////  Begin  /////////
        ///////////////////////////

        for (int i = 0; i < border.tri.size(); i++)
        {
            ImTri temp = border.tri[i];
            ImVec2 cent = (temp.a + temp.b + temp.c) / 3;
            ImVec2 ca = ~(temp.a - cent);
            ImVec2 cb = ~(temp.b - cent);
            ImVec2 cc = ~(temp.c - cent);
            drawlist->AddTriangleFilled((temp.a + ca) * SCREENSCALE, (temp.b + cb) * SCREENSCALE, (temp.c + cc) * SCREENSCALE, COLOR0);
        }
        for (int i = 0; i < barrierlist.size(); i++)
        {
            for (int j = 0; j < barrierlist[i].tri.size(); j++)
            {
                ImTri temp = barrierlist[i].tri[j];
                ImVec2 cent = (temp.a + temp.b + temp.c) / 3;
                ImVec2 ca = ~(temp.a - cent);
                ImVec2 cb = ~(temp.b - cent);
                ImVec2 cc = ~(temp.c - cent);
                if (i + 1 == barrierkk) drawlist->AddTriangleFilled((temp.a + ca) * SCREENSCALE, (temp.b + cb) * SCREENSCALE, (temp.c + cc) * SCREENSCALE, DARK1);
                else drawlist->AddTriangleFilled((temp.a + ca) * SCREENSCALE, (temp.b + cb) * SCREENSCALE, (temp.c + cc) * SCREENSCALE, DARK0);
            }
        }
        drawlist->AddCircleFilled(bend * SCREENSCALE, STANDSIZE * SCREENSCALE, END1, STANDPOINTEDGENUM);
        drawlist->AddCircleFilled(bstart * SCREENSCALE, STANDSIZE * SCREENSCALE, START1, STANDPOINTEDGENUM);
        if (bordersetting)
        {
            for (int i = 0; i < newborder.line.size(); i++)
            {
                ImLine temp = newborder.line[i];
                drawlist->AddLine(temp.a * SCREENSCALE, temp.b * SCREENSCALE, BLUE1, BORDERSIDEWIDTH * SCREENSCALE);
            }
            for (int i = 0; i < newborder.vert.size(); i++)
            {
                ImVec2 temp = newborder.vert[i];
                drawlist->AddCircleFilled(temp * SCREENSCALE, BORDERPOINTSIZE * SCREENSCALE, BLUE1, BORDERPOINTEDGENUM);
            }
            drawlist->AddCircleFilled(mouse * SCREENSCALE, BORDERPOINTSIZE * SCREENSCALE, BLUE0, BORDERPOINTEDGENUM);
        }
        if (barriersetting)
        {
            for (int i = 0; i < newbarrier.line.size(); i++)
            {
                ImLine temp = newbarrier.line[i];
                drawlist->AddLine(temp.a * SCREENSCALE, temp.b * SCREENSCALE, RED1, BORDERSIDEWIDTH * SCREENSCALE);
            }
            for (int i = 0; i < newbarrier.vert.size(); i++)
            {
                ImVec2 temp = newbarrier.vert[i];
                drawlist->AddCircleFilled(temp * SCREENSCALE, BORDERPOINTSIZE * SCREENSCALE, RED1, BORDERPOINTEDGENUM);
            }
            drawlist->AddCircleFilled(mouse * SCREENSCALE, BORDERPOINTSIZE * SCREENSCALE, RED0, BORDERPOINTEDGENUM);
        }
        if (endsetting) drawlist->AddCircleFilled(mouse * SCREENSCALE, STANDSIZE * SCREENSCALE, END0, STANDPOINTEDGENUM);
        if (startsetting) drawlist->AddCircleFilled(mouse * SCREENSCALE, STANDSIZE * SCREENSCALE, START0, STANDPOINTEDGENUM);

        if (!thisignal)
        {
            if (!dynamik) DrawPerson(drawlist);
            else DrawPeople(drawlist);
            if (area->kk != 0) drawlist->AddCircleFilled(area->pos * SCREENSCALE, BORDERPOINTSIZE, BLUE0, BORDERPOINTEDGENUM);
        }
        ///////////////////////////
        /////////   End   /////////
        ///////////////////////////

        ImGui::End();

        ImGui::Render();
        ImDrawData* drawData = ImGui::GetDrawData();
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(drawData);
        glReadPixels(0, INDEXY, CANVASX, CANVASY, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
        Repixel();
        glfwSwapBuffers(window);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        deltatime = playspeed * 0.001f * static_cast<float>(duration.count());
        if (playing) playtime -= deltatime;
        if (playtime < 0) playing = dynamik = false;
    }
    makefinish = true;
    for (int i = 0; i < pool.size(); i++)
        pool[i].join();
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    shader.delete_program();
    glfwTerminate();
    return 0;
}
