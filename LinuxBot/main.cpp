#include <dpp/dpp.h>

const std::string BOT_TOKEN = "";

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

 // embed build example, as a reply
void fridayFunc(dpp::slashcommand_t event)
{

	/* create the embed */
	dpp::embed embed = dpp::embed().
		set_color(dpp::colors::sti_blue).
		set_title("***WHEN IT FRIDAY***").
		set_url("https://youtu.be/_2U-0fAe-QQ").
		//set_author("Some name", "https://dpp.dev/", "https://dpp.dev/DPP-Logo.png").
		//set_description("Some description here").
	   //set_thumbnail("https://dpp.dev/DPP-Logo.png").

		set_image("https://cdn.discordapp.com/attachments/656751148379668500/1013462947906994229/today-is.gif").
		set_footer(dpp::embed_footer().set_text("gaming").set_icon("https://cdn.discordapp.com/attachments/656751148379668500/1013464958429839410/every_secon.gif")).
		set_timestamp(time(0));

	/* reply with the created embed */
	event.reply(dpp::message(event.command.channel_id, embed).set_reference(event.command.message_id));

}

// embed build example
void fridayFunc(dpp::message_create_t event, dpp::cluster& bot)
{

	/* create the embed */
	dpp::embed embed = dpp::embed().
		set_color(dpp::colors::sti_blue).
		set_title("***WHEN IT FRIDAY***").
		set_url("https://youtu.be/_2U-0fAe-QQ").
		//set_author("Some name", "https://dpp.dev/", "https://dpp.dev/DPP-Logo.png").
		//set_description("Some description here").
	   //set_thumbnail("https://dpp.dev/DPP-Logo.png").

		set_image("https://cdn.discordapp.com/attachments/656751148379668500/1013462947906994229/today-is.gif").
		set_footer(dpp::embed_footer().set_text("gaming").set_icon("https://cdn.discordapp.com/attachments/656751148379668500/1013464958429839410/every_secon.gif")).
		set_timestamp(time(0));

	/* reply with the created embed */
	bot.message_create(dpp::message(event.msg.channel_id, embed).set_reference(event.msg.id));

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
            fridayFunc(event, bot);
        }
        if (event.msg.content == "!ping") {
            event.reply("Pong!");
        }

        addXP(event.msg.author);

        });

    // adding a slash commands
    bot.on_ready([&bot](const dpp::ready_t& event) {
        // commands registration are in in run_once to make sure it doesnt run on every full reconnection
        if (dpp::run_once<struct register_bot_commands>()) {
            bot.global_command_create(dpp::slashcommand("ping", "Ping pong!", bot.me.id));

            dpp::slashcommand rankcommand("rank", "Get your or anouther members rank.", bot.me.id);
            rankcommand.add_option(
                dpp::command_option(dpp::co_mentionable, "Member", "optional", false)
            );
            bot.global_command_create(rankcommand);

            bot.global_command_create(dpp::slashcommand("topranks", "Show a list of members with highest ranks.", bot.me.id));
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
            dpp::interaction interaction = event.command;
            dpp::command_interaction cmd_data = interaction.get_command_interaction();
            auto subcommand = cmd_data.options.at(0);
            if (subcommand.name == "Member") {
                if (!subcommand.options.empty()) {
                    event.reply("> XP: " + std::to_string(getXP(event.command.get_issuing_user())));
                }
                else {
                    event.reply("XP: "+std::to_string(getXP(event.command.get_issuing_user())));
                }
            }
            else
                event.reply("XP: " + std::to_string(getXP(event.command.get_issuing_user())));
        }
        if (event.command.get_command_name() == "topranks")
        {

        }

    });


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
        if (sec == 2)
        {
            sec = 0;

            applyXP();
            saveData(getDatabase());
        }
    };

    return 0;
}