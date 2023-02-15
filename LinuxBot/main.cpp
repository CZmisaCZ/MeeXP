#include <dpp/dpp.h>

#include <string>
#include <stdio.h>
#include <time.h>
#include "fileIO.h"
#include "event.h"
#include "settings.h"

#ifdef __unix__

#include <unistd.h>
unsigned int microsecond = 250000;//250ms

#endif

#ifdef _WIN32

//#pragma warning(disable:4996)

#include <chrono>
#include <thread>

#endif

void levelupEmbed(dpp::message_create_t message)
{
    RankData data = getRank(message.msg.author);

    std::string desc;

    desc = "***YEEEHAAAW*** " + message.msg.author.get_mention() + ", reached "+std::to_string(data.lvl) + ". level\n\n" +
        "Rank: **" + std::to_string(data.rank) + "#**\n" +
        "Level: **" + std::to_string(data.lvl) + " **\n\n";

    desc += "**[:yellow_square::yellow_square::yellow_square::yellow_square::yellow_square::yellow_square::yellow_square::yellow_square::yellow_square::yellow_square:]**\n";

    desc += "*" + std::to_string(data.xp) + "xp / " + std::to_string(data.xptothislvl) + "xp*\n";

    //create embed
    dpp::embed embed = dpp::embed().
        set_color(dpp::colors::moon_yellow).
        set_description(desc).
        set_footer(dpp::embed_footer().set_text("let's do this texas style !").set_icon("https://cdn.discordapp.com/attachments/656751148379668500/1013464958429839410/every_secon.gif")).
        set_timestamp(time(0));

    // send embed as reply
    message.reply(dpp::message(message.msg.channel_id, embed).set_reference(message.msg.id));
}

void rankEmbed(dpp::slashcommand_t event, dpp::user user)
{
    RankData data = getRank(user);

    std::string desc;

    desc = "__" + user.get_mention() + "__\n\n" +
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

    if (user.is_bot() == true)desc += "***bots dont get XP or levels***";

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
        
        table = table + "**" +std::to_string(i+1) + "#** " + a.get_mention() + " " + std::to_string(top.at(i)->xp) + "xp\n";
    }

    //create embed
    dpp::embed embed = dpp::embed().
        set_color(dpp::colors::moon_yellow).
        set_title("__Top Ranks__").
        //set_url("https://docs.google.com/spreadsheets/d/10s70KvJEFxrwb-bulRzP-r17Z6ZdSnH7lH5AV_DC0bM/edit#gid=0").
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
    dpp::cluster bot(sett::BOT_TOKEN, dpp::i_default_intents | dpp::i_message_content);

    // output simple log messages to stdout
    if (sett::print)bot.on_log(dpp::utility::cout_logger());

    // message handler to look for a command called !ping ...
    bot.on_message_create([&bot](const dpp::message_create_t& event) {

        if (event.msg.content == "!rank" && event.msg.is_dm() == false) {
            event.reply("Use: /rank");
        }

        if (event.msg.content == "!ping") {
            event.reply("Pong!");
        }

        if (addXP(event.msg.author))
        {
            if (issielent(event.msg.author)==false)
            levelupEmbed(event);
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

            bot.global_command_create(dpp::slashcommand("levels", "Show a list of members with highest ranks.", bot.me.id));

            //bot.global_command_create(dpp::slashcommand("xpneed", "Show how many xp do you need for ranks form 0 to 100.", bot.me.id));

            dpp::slashcommand setxpcommand("setxp", "Sets xp of user (this will overrite their XP and level), admin only command.", bot.me.id);
            setxpcommand.add_option(dpp::command_option(dpp::co_user, "user", "Mention user to give xp to.", true));
            setxpcommand.add_option(dpp::command_option(dpp::co_number, "xp", "How many xp to give. (any number between 0 and 9007199254740992)", true));
            bot.global_command_create(setxpcommand);

            dpp::slashcommand givexpcommand("givexp", "Gives xp to a user, admin only command.", bot.me.id);
            givexpcommand.add_option(dpp::command_option(dpp::co_user, "user", "Mention user to give xp to.", true));
            givexpcommand.add_option(dpp::command_option(dpp::co_number, "xp", "How many xp to give. (any number between 0 and 9007199254740992)", true));
            bot.global_command_create(givexpcommand);

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
            // check if there is "user" imput
            dpp::interaction interaction = event.command;
            dpp::command_interaction cmd_data = interaction.get_command_interaction();
            bool subcommand = cmd_data.options.size();

            if (subcommand)
            {
                uint64_t ID = std::get<dpp::snowflake>(event.get_parameter("user"));
                if (isUserInDatabase(ID))
                rankEmbed(event, *dpp::find_user(ID));
                else
                    event.reply("User has no XP.");
            }
            else
                rankEmbed(event, event.command.get_issuing_user());
        }
        if (event.command.get_command_name() == "levels")
        {
            topRanksEmbed(event);
        }
        if (event.command.get_command_name() == "xpneed")
        {
            xpNeededEmbed(event);
        }
        if (event.command.get_command_name() == "silence")
        {
            sielence(event.command.get_issuing_user());
            event.reply(":thumbsup:");
        }
        if (event.command.get_command_name() == "setxp")
        {
            if (ismoderator(event.command.get_issuing_user()))
            {
                uint64_t ID = std::get<dpp::snowflake>(event.get_parameter("user"));
                if (isUserInDatabase(ID))
                {
                    setXP(*dpp::find_user(ID), std::get<double>(event.get_parameter("xp")));
                    event.reply(":thumbsup:");
                }
                else
                {
                    event.reply("User is not in database, user must type something first if you want to give him XP.");
                }
            }
            else
            {
                event.reply("This is admin only command.");
            }
        }
        if (event.command.get_command_name() == "givexp")
        {
            if (ismoderator(event.command.get_issuing_user()))
            {
                uint64_t ID = std::get<dpp::snowflake>(event.get_parameter("user"));
                if (isUserInDatabase(ID))
                {
                    giveXP(*dpp::find_user(std::get<dpp::snowflake>(event.get_parameter("user"))), std::get<double>(event.get_parameter("xp")));
                    event.reply(":thumbsup:");
                }
                else
                {
                    event.reply("User is not in database, user must type something first if you want to give him XP.");
                }
            }
            else
            {
                event.reply("This is admin only command.");
            }
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

    unsigned int min=0;
    unsigned int tim=0;

    while (true)
    {
        #ifdef _WIN32
        {
            sleep_for(250ms);
        }
        #endif

        #ifdef __unix__
        {
            usleep(microsecond);
        }
        #endif

        min++;
        //apply xp every minute
        if (min == 4*60)
        {
            tim++;
            min = 0;

            applyXP();
        }
        //save every set time in settings (default 1 min)
        if (tim == 4*60*sett::saveinterval)
        {
            saveData(getDatabase(),-1);
        }
    };

    return 0;
}
