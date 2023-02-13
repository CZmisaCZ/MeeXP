#include <dpp/dpp.h>

const std::string BOT_TOKEN = "MTAxMzcyOTk2ODI2NzczNTA4MA.GSCbqc.QvfAw-mae5Tl3ZXEXNNHvNgtLtwYNZ6kRozREA";

#include <string>
#include <stdio.h>
#include <time.h>
#include "fileIO.h"
#include "event.h"

#ifdef linux

#include <unistd.h>
unsigned int microsecond = 250000;//250ms

#endif

#ifdef _WIN32

//#pragma warning(disable:4996)

#include <chrono>
#include <thread>

#endif

void levelupEmbed(dpp::message_create_t event, dpp::cluster& bot)
{
    //create embed
    dpp::embed embed = dpp::embed().
        set_color(dpp::colors::sti_blue).
        set_title("***LEVEL UP***").

        set_image("https://cdn.discordapp.com/attachments/656751148379668500/1013462947906994229/today-is.gif").
        set_footer(dpp::embed_footer().set_text("gaming").set_icon("https://cdn.discordapp.com/attachments/656751148379668500/1013464958429839410/every_secon.gif")).
        set_timestamp(time(0));

    // send embed as reply
    bot.message_create(dpp::message(event.msg.channel_id, embed).set_reference(event.msg.id));
}

void rankEmbed(dpp::slashcommand_t event)
{
    RankData data = getRank(event.command.get_issuing_user().id);

    std::string desc;

    desc = "__" + event.command.get_issuing_user().get_mention() + "__\n\n" +
        "Rank: **" + std::to_string(data.rank) + "#**\n" +
        "Level: **" + std::to_string(data.lvl) + " **\n\n";

    desc += "**[";

    float bars = ((float)(data.xp - data.xptothislvl) / (float)(data.xptonextlvl - data.xptothislvl))*10.0f;
    //generate bar
    for (auto i = 0; i < 10; i++)
    {
        if (bars >= 1)
        {
            bars += -1;
            desc += ":yellow_square:";
        }
        else if (bars > 0)
        {
            if (bars > 0.75f)
            {
                desc += ":white_large_square:";
            }
            else if (bars > 0.50f)
            {
                desc += ":white_medium_square:";
            }
            else if (bars > 0.25f)
            {
                desc += ":white_medium_small_square:";
            }
            else
            {
                desc += ":white_small_square:";
            }
            bars = 0;
        }
        else
        {
            desc += ":black_large_square:";
        }
    }

    desc += "]**\n";

    desc += "*"+std::to_string(data.xp) + "xp / " + std::to_string(data.xptonextlvl) + "xp*\n";


    //create embed
    dpp::embed embed = dpp::embed().
        set_color(dpp::colors::moon_yellow).
        //set_title(event.command.get_issuing_user().get_mention()).
        set_description(desc).
        set_footer(dpp::embed_footer().set_text("gaming").set_icon("https://cdn.discordapp.com/attachments/656751148379668500/1013464958429839410/every_secon.gif")).
        set_timestamp(time(0));

    // send embed as reply
    event.reply(dpp::message(event.command.channel_id, embed).set_reference(event.command.message_id));
}

void xpNeededEmbed(dpp::slashcommand_t event)
{
    std::string table;

    unsigned long long xp=0;
    for (auto lvl =0; lvl <= 100; lvl++)
    {
        xp += (5 * pow(lvl, 2) + 50 * lvl + 100);
        table = table + "**rank: " + std::to_string(lvl) + "**\nxp: " + std::to_string(xp)+"\n";
    }

    //create embed
    dpp::embed embed = dpp::embed().
        set_color(dpp::colors::moon_yellow).
        set_title("__0 to 100 ranks and xp needed to rank up__").
        set_url("https://docs.google.com/spreadsheets/d/10s70KvJEFxrwb-bulRzP-r17Z6ZdSnH7lH5AV_DC0bM/edit#gid=0").
        set_description(table).
        set_footer(dpp::embed_footer().set_text("gaming").set_icon("https://cdn.discordapp.com/attachments/656751148379668500/1013464958429839410/every_secon.gif")).
        set_timestamp(time(0));

    // send embed as reply
    event.reply(dpp::message(event.command.channel_id, embed).set_reference(event.command.message_id));

}

void topRanksEmbed(dpp::slashcommand_t event)
{
    //getDatabase();
    std::vector<UserXP*> top = getTopDatabase(10);
    std::string table;

    for (auto i = 0; i < top.size(); i++)
    {
        dpp::user a;
        a.id = top.at(i)->userID;
        
        table = table + std::to_string(top.at(i)->xp) + "xp " + a.get_mention() + "\n";
    }

    //create embed
    dpp::embed embed = dpp::embed().
        set_color(dpp::colors::moon_yellow).
        set_title("__top users__").
        set_url("https://docs.google.com/spreadsheets/d/10s70KvJEFxrwb-bulRzP-r17Z6ZdSnH7lH5AV_DC0bM/edit#gid=0").
        set_description(table).
        set_footer(dpp::embed_footer().set_text("gaming").set_icon("https://cdn.discordapp.com/attachments/656751148379668500/1013464958429839410/every_secon.gif")).
        set_timestamp(time(0));

    // send embed as reply
    event.reply(dpp::message(event.command.channel_id, embed).set_reference(event.command.message_id));

}

int main()
{
    // time as seed for random generator used in addRandomXP();
    srand(time(NULL));

    // create bot cluster
    dpp::cluster bot(BOT_TOKEN, dpp::i_default_intents | dpp::i_message_content);

    // output simple log messages to stdout
    bot.on_log(dpp::utility::cout_logger());

    // message handler to look for a command called !ping ...
    bot.on_message_create([&bot](const dpp::message_create_t& event) {
        if (event.msg.content == "!rank" && event.msg.is_dm() == false) {
            event.reply("use /rank");
        }
        if (event.msg.content == "!ping") {
            event.reply("Pong!");
        }

        if (addXP(event.msg.author))
        {
            levelupEmbed(event, bot);
        }

    });

    // adding a slash commands
    bot.on_ready([&bot](const dpp::ready_t& event) {
        // commands registration are in in run_once to make sure it doesnt run on every full reconnection
        if (dpp::run_once<struct register_bot_commands>()) {
            bot.global_command_create(dpp::slashcommand("ping", "Ping pong!", bot.me.id));

            dpp::slashcommand rankcommand("rank", "Get your or anouther members rank.", bot.me.id);
            rankcommand.add_option(
                dpp::command_option(dpp::co_user, "user", "Mention user. (optional)",false)
            );

            bot.global_command_create(rankcommand);

            bot.global_command_create(dpp::slashcommand("topranks", "Show a list of members with highest ranks.", bot.me.id));

            bot.global_command_create(dpp::slashcommand("xpneed", "Show how many xp do you need for ranks form 0 to 100.", bot.me.id));

            bot.global_command_create(dpp::slashcommand("silence", "Turn off level up messages for yourself.", bot.me.id));
        }
    });


    // slash command handler
    bot.on_slashcommand([](const dpp::slashcommand_t& event) {
        if (event.command.get_command_name() == "ping")
        {
            event.reply("Pong!");
        }
        if (event.command.get_command_name() == "rank")
        {
            rankEmbed(event);
        }
        if (event.command.get_command_name() == "topranks")
        {
            topRanksEmbed(event);
        }
        if (event.command.get_command_name() == "xpneed")
        {
            xpNeededEmbed(event);
        }

    });

    setDatabase(loadData());

    bot.start(true);

    #ifdef _WIN32
    using namespace std::this_thread;     // sleep_for, sleep_until
    using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
    using std::chrono::system_clock;
    using namespace std::chrono;
    auto start = high_resolution_clock::now();
    #endif

    int sec=0;

    while (true)
    {
        #ifdef _WIN32
        {
            sleep_for(250ms);
        }
        #endif

        #ifdef linux
        {
            usleep(microsecond);
        }
        #endif

        sec++;
        if (sec == 4)
        {
            sec = 0;

            applyXP();
            saveData(getDatabase());
        }
    };

    return 0;
}