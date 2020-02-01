// Ginger Run
// by Dr. Jonathan Metzgar et al
// UAF CS Game Design and Architecture Course
#include <gamelib.hpp>
#include <gamelib_random.hpp>

#pragma comment(lib, "gamelib.lib")

class PlaySoundCommand : public GameLib::InputCommand {
public:
    PlaySoundCommand(int audioClipId, bool stopPrevious)
        : musicClipId_{ audioClipId }
        , stopPrevious_{ stopPrevious } {}

    const char* type() const override { return "PlaySoundCommand"; }

    bool execute(float amount) override {
        GameLib::Locator::getAudio()->playAudio(musicClipId_, stopPrevious_);
        return true;
    }

private:
    int musicClipId_{ 0 };
    bool stopPrevious_{ false };
};

class PlayMusicCommand : public GameLib::InputCommand {
public:
    PlayMusicCommand(int musicClipId)
        : musicClipId_{ musicClipId } {}

    const char* type() const override { return "PlayMusicCommand"; }

    bool execute(float amount) override {
        GameLib::Locator::getAudio()->playMusic(musicClipId_, -1, 0);
        return true;
    }

private:
    int musicClipId_{ 0 };
    bool stopPrevious_{ false };
};

class QuitCommand : public GameLib::InputCommand {
public:
    const char* type() const override { return "QuitCommand"; }
    bool execute(float amount) override {
        GameLib::Locator::getContext()->quitRequested = true;
        return true;
    }
};

class MovementCommand : public GameLib::InputCommand {
public:
    const char* type() const override { return "MovementCommand"; }
    bool execute(float amount) override {
        // apply slight curve
        if (amount < 0.1f && amount > -0.1f)
            amount = 0.0f;
        else if (amount > 0.5f)
            amount = 1.0f;
        else if (amount < -0.5f)
            amount = -1.0f;
        else if (amount > 0.0f)
            amount = 0.5f;
        else
            amount = -0.5f;
        return InputCommand::execute(amount);
    }
};

class GBRandomInputComponent : public GameLib::InputComponent {
public:
    virtual ~GBRandomInputComponent() {}
    void update(GameLib::Actor& actor) override {
        if (counter_ < GameLib::Context::currentTime_s) {
            counter_ = GameLib::Context::currentTime_s + 1.0f;
            actor.velocity.x = GameLib::random.normal();
            actor.velocity.y = GameLib::random.normal();
            glm::normalize(actor.velocity);
        }
    }

private:
    float counter_{ 0 };
};

namespace GameLib {
    class Font {
    public:
        static constexpr int HALIGN_LEFT = 0;
        static constexpr int HALIGN_CENTER = 1 << 0;
        static constexpr int HALIGN_RIGHT = 1 << 1;
        static constexpr int VALIGN_TOP = 1 << 2;
        static constexpr int VALIGN_CENTER = 1 << 3;
        static constexpr int VALIGN_BOTTOM = 1 << 4;
        static constexpr int SHADOWED = 1 << 5;

        Font(Context* context)
            : context_(context) {}
        ~Font();
        bool load(const std::string& path, int ptsize);
        SDL_Texture* render(const char* text, SDL_Color fg);
        SDL_Texture* lastRender() { return texture_; }
        void newRender() {
            if (texture_) {
                SDL_DestroyTexture(texture_);
                texture_ = nullptr;
            }
            if (surface_) {
                SDL_FreeSurface(surface_);
                surface_ = nullptr;
            }
        }

        // calculates the width of the string text
        int calcWidth(const char* text) {
            int w{ 0 };
            if (font_)
                TTF_SizeUTF8(font_, text, &w, nullptr);
            return w;
        }

        // calculates the height of the loaded font
        int calcHeight() const { return TTF_FontHeight(font_); }

        // returns the height of the rendered string
        int height() const { return rect_.h; }
        // returns the width of the renderered string
        int width() const { return rect_.w; }

        void draw(int x, int y);

        void draw(int x, int y, const char* text, SDL_Color fg, int flags) {
            if (flags & HALIGN_CENTER) {
                x -= calcWidth(text) >> 1;
            } else if (flags & HALIGN_RIGHT) {
                x -= calcWidth(text);
            }
            if (flags & VALIGN_TOP) {
            } else if (flags & VALIGN_CENTER) {
                y -= calcHeight() >> 1;
            } else if (flags & VALIGN_BOTTOM) {
                y -= calcHeight();
            }
            if (flags & SHADOWED) {
                render(text, Black);
                draw(x + 2, y + 2);
            }
            render(text, fg);
            draw(x, y);
        }

    private:
        Context* context_{ nullptr };
        TTF_Font* font_{ nullptr };
        SDL_Texture* texture_{ nullptr };
        SDL_Surface* surface_{ nullptr };
        SDL_Rect rect_;
    };

    Font::~Font() {
        if (font_) {
            TTF_CloseFont(font_);
            font_ = nullptr;
        }
    }

    bool Font::load(const std::string& filename, int ptsize) {
        std::string path = context_->findSearchPath(filename);
        font_ = TTF_OpenFont(path.c_str(), ptsize);
        return font_ != nullptr;
    }

    SDL_Texture* Font::render(const char* text, SDL_Color fg) {
        if (!font_)
            return nullptr;
        newRender();
        surface_ = TTF_RenderText_Blended(font_, text, fg);
        if (surface_) {
            rect_.w = surface_->w;
            rect_.h = surface_->h;
            texture_ = SDL_CreateTextureFromSurface(context_->renderer(), surface_);
        }
        return texture_;
    }

    void Font::draw(int x, int y) {
        rect_.x = x;
        rect_.y = y;
        SDL_Renderer* renderer_ = context_->renderer();
        SDL_RenderCopy(renderer_, texture_, nullptr, &rect_);
    }
}

void testSprites(GameLib::Context& context, int spriteCount, int& spritesDrawn, SDL_Texture* testPNG, SDL_Texture* testJPG);

int main(int argc, char** argv) {
    GameLib::Context context(1280, 720, GameLib::WindowDefault);
    GameLib::Audio audio;
    GameLib::InputHandler input;
    GameLib::Graphics graphics{ &context };

    GameLib::Locator::provide(&context);
    if (context.audioInitialized())
        GameLib::Locator::provide(&audio);
    GameLib::Locator::provide(&input);
    GameLib::Locator::provide(&graphics);

    PlaySoundCommand play0(0, false);
    PlaySoundCommand play1(1, false);
    PlaySoundCommand play2(2, false);
    PlaySoundCommand play3(3, false);
    PlayMusicCommand playMusic1(0);
    PlayMusicCommand playMusic2(1);
    PlayMusicCommand playMusic3(2);
    QuitCommand quitCommand;
    MovementCommand xaxisCommand;
    MovementCommand yaxisCommand;

    input.back = &quitCommand;
    input.key1 = &play0;
    input.key2 = &play1;
    input.key3 = &play2;
    input.key4 = &play3;
    input.key5 = &playMusic1;
    input.key6 = &playMusic2;
    input.key7 = &playMusic3;
    input.axis1X = &xaxisCommand;
    input.axis1Y = &yaxisCommand;

    context.addSearchPath("./assets");
    context.addSearchPath("../assets");
    SDL_Texture* testPNG = context.loadImage("godzilla.png");
    SDL_Texture* testJPG = context.loadImage("parrot.jpg");
    graphics.setTileSize(32, 32);
    int spriteCount = context.loadTileset(0, 32, 32, "Tiles32x32.png");
    if (!spriteCount) {
        HFLOGWARN("Tileset not found");
    }
    spriteCount = context.loadTileset(1, 32, 32, "uglygingerbreadman.png");

    context.loadAudioClip(0, "starbattle-bad.wav");
    context.loadAudioClip(1, "starbattle-dead.wav");
    context.loadAudioClip(2, "starbattle-endo.wav");
    context.loadAudioClip(3, "starbattle-exo.wav");
    context.loadAudioClip(4, "starbattle-ok.wav");
    context.loadAudioClip(5, "starbattle-pdead.wav");
    context.loadMusicClip(1, "starbattlemusic1.mp3");
    context.loadMusicClip(0, "GingerRun.mp3");
    context.loadMusicClip(2, "distoro2.mid");

    GameLib::Font gothicfont(&context);
    GameLib::Font minchofont(&context);
    GameLib::Font minchofont72(&context);
    gothicfont.load("fonts-japanese-gothic.ttf", 36);
    minchofont.load("fonts-japanese-mincho.ttf", 36);
    minchofont72.load("URWClassico-BoldItalic.ttf", 72);

    GameLib::World world;
    GameLib::Locator::provide(&world);
    std::string worldPath = context.findSearchPath("world.txt");
    if (!world.load(worldPath)) {
        HFLOGWARN("world.txt not found");
    }

    Hf::StopWatch stopwatch;
    double spritesDrawn = 0;
    double frames = 0;
    GameLib::Actor player(new GameLib::SimpleInputComponent(),
                          new GameLib::SimpleActorComponent(),
                          new GameLib::SimplePhysicsComponent(),
                          new GameLib::SimpleGraphicsComponent());
    player.speed = (float)graphics.getTileSizeX();
    player.position.x = graphics.getCenterX() / (float)graphics.getTileSizeX();
    player.position.y = graphics.getCenterY() / (float)graphics.getTileSizeY();
    player.spriteId = 2;

    // GameLib::MoveAction moveAction;
    // moveAction.setActor(&player);

    world.actors.push_back(&player);

    GameLib::Actor randomPlayer(new GBRandomInputComponent(),
                                new GameLib::SimpleActorComponent(),
                                new GameLib::SimplePhysicsComponent(),
                                new GameLib::SimpleGraphicsComponent());

    world.actors.push_back(&randomPlayer);
    randomPlayer.position.x = graphics.getCenterX() / (float)graphics.getTileSizeX();
    randomPlayer.position.y = graphics.getCenterY() / (float)graphics.getTileSizeY();
    randomPlayer.spriteLibId = 1;
    randomPlayer.spriteId = 0;
    randomPlayer.speed = (float)graphics.getTileSizeX();

    float t0 = stopwatch.Stop_sf();

	context.playMusicClip(0);
    while (!context.quitRequested) {
        float t1 = stopwatch.Stop_sf();
        float dt = t1 - t0;
        t0 = t1;
        GameLib::Context::deltaTime = dt;
        GameLib::Context::currentTime_s = t1;
        GameLib::Context::currentTime_ms = t1 * 1000;

        context.getEvents();
        input.handle();

        context.clearScreen(GameLib::Azure);

        for (unsigned x = 0; x < world.worldSizeX; x++) {
            for (unsigned y = 0; y < world.worldSizeY; y++) {
                GameLib::SPRITEINFO s;
                s.position = { x * 32, y * 32 };
                auto t = world.getTile(x, y);
                context.drawTexture(s.position, 0, t.charDesc);
            }
        }

        world.update(dt, graphics);

        minchofont.draw(0, 0, "Hello, world!", GameLib::Red, GameLib::Font::SHADOWED);
        gothicfont.draw((int)graphics.getWidth(), 0, "Hello, world!", GameLib::Blue, GameLib::Font::HALIGN_RIGHT | GameLib::Font::SHADOWED);

        int x = (int)graphics.getCenterX();
        int y = (int)graphics.getCenterY();
        float s = GameLib::wave(t1, 1.0f);
        SDL_Color c = GameLib::MakeColorHI(7, 4, s, false);
        minchofont72.draw(x, y, "GingerRun", c, GameLib::Font::SHADOWED | GameLib::Font::HALIGN_CENTER | GameLib::Font::VALIGN_CENTER);

        minchofont.draw(0, (int)graphics.getHeight() - 2, "HP: 56", GameLib::Gold, GameLib::Font::VALIGN_BOTTOM | GameLib::Font::SHADOWED);

        context.swapBuffers();
        frames++;
        std::this_thread::yield();
    }
    double totalTime = stopwatch.Stop_s();
    HFLOGDEBUG("Sprites/sec = %5.1f", spritesDrawn / totalTime);
    HFLOGDEBUG("Frames/sec = %5.1f", frames / totalTime);

    return 0;
}

void testSprites(GameLib::Context& context, int spriteCount, int& spritesDrawn, SDL_Texture* testPNG, SDL_Texture* testJPG) {
    // if (context.keyboard.scancodes[SDL_SCANCODE_ESCAPE]) {
    //    context.quitRequested = true;
    //}

    // An arbitrary number roughly representing 4k at 8 layers, 32x32 sprites
    // constexpr int SpritesToDraw = 128 * 72 * 8;
    // An arbitrary number roughly representing HD at 4 layers, 32x32 sprites
    constexpr int SpritesToDraw = 5;
    // 60 * 34 * 4;
    for (int i = 0; i < SpritesToDraw; i++) {
        GameLib::SPRITEINFO s;
        s.position = { rand() % 1280, rand() % 720 };
        s.center = { 0.0f, 0.0f };
        s.flipFlags = 0;
        s.angle = (float)(rand() % 360);
        // context.drawTexture(0, rand() % spriteCount, s);
        s.position = { rand() % 1280, rand() % 720 };
        context.drawTexture(s.position, 0, rand() % spriteCount);
    }
    spritesDrawn += SpritesToDraw;

    context.drawTexture({ 50, 0 }, { 100, 100 }, testPNG);
    context.drawTexture({ 250, 250 }, { 100, 100 }, testJPG);
}