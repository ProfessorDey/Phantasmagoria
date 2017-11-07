#include "Game.h"
#include "Runtime.h"
#include "Event.h"

enum Events {
	EMPTY,
	ENTRYHALL,
	CARRIAGE,
	STAGE,
	STAGEEXECUTED,
	STAGESPARED,
	BACKSTAGE,
	CAVE,
	CAVECLEAR,
	DEN,
	DENCLEAR,
	WORKSHOP,
	CUBBYHOLE,
	TORMENTOR,
	CLUBEVENT,
	CURSEDBONE,
	BLANKETEVENT,
	TICKETTAKEN,
	SLEEP
};

enum Items {
	NOTHING,
	BONE,
	DICE,
	CLUB,
	HAT,
	ARMOUR,
	BLANKET,
	TICKET
};

int Event::Play(int eventID) {
	run.rend.ClearScreen();
	if (defaultMenu()) return -1;
	switch (eventID) {
	case EMPTY: return Empty();
	case ENTRYHALL: return EntryHall();
	case CARRIAGE: return Carriage();
	case STAGE: return Stage();
	case STAGEEXECUTED: return StageExecuted();
	case STAGESPARED: return StageSpared();
	case BACKSTAGE: return BackStage();
	case CAVE: return Cave();
	case CAVECLEAR: return CaveClear();
	case DEN: return Den();
	case DENCLEAR: return DenClear();
	case WORKSHOP: return Workshop();
	case CUBBYHOLE: return CubbyHole();
	case TORMENTOR: return Tormentor();
	case CLUBEVENT: return Club();
	case CURSEDBONE: return CursedBone();
	case BLANKETEVENT: return Blanket();
	case TICKETTAKEN: return Ticket();
	case SLEEP: return Sleep();
	default: break;
	}
	return -1;
}

int Event::GenericRoom(std::string description) {
	int roomID = run.p.Location(0, 0);
	int itemCount = run.dungeon.room[roomID].floor.SlotsTaken(0, 0);
	switch (itemCount) {
	case 0: // If room is empty, add below to room description
		description += "the floor is bare, save for dust and broken furniture.";
		break;
	case 1: // If room has a single Item, add the following
		description += "the floor is slightly messy, but devoid of useful items, or so you believe, until you see a peculiar item below the piles of broken furniture.";
		break;
	default: // If a room has multiple items, add this instead
		description += "the floor is littered with detritus of all kinds, and immediately a couple of unusual items catch your eye.";
		description += newLine(" ", 72) + "First amongst these items is ";
		description += run.itemList[run.dungeon.room[roomID].floor.GetItem(i)].Description(0, "");
		}
	}
	run.rend.DrawText(description);
	int valid = 0;
	while (!valid) {
		run.rend.DrawNavigationPrompt();
		int sel = run.parse.Selection("north west south east take swap drop character exit");
		switch (sel) {
		case 0: if (run.dungeon.room[roomID].Direction('N', 0, 0) != -1) { run.p.Location(1, run.dungeon.room[roomID].Direction('N', 0, 0)); valid = !valid; } break;
		case 1: if (run.dungeon.room[roomID].Direction('W', 0, 0) != -1) { run.p.Location(1, run.dungeon.room[roomID].Direction('W', 0, 0)); valid = !valid; } break;
		case 2: if (run.dungeon.room[roomID].Direction('S', 0, 0) != -1) { run.p.Location(1, run.dungeon.room[roomID].Direction('S', 0, 0)); valid = !valid; } break;
		case 3: if (run.dungeon.room[roomID].Direction('E', 0, 0) != -1) { run.p.Location(1, run.dungeon.room[roomID].Direction('E', 0, 0)); valid = !valid; } break;
		case 4: if (run.dungeon.room[roomID].floor.SlotsTaken(0, 0)) { ItemMenu(0); valid = !valid; } break;
		case 5: if (run.p.inventory.SlotsTaken(0, 0) && run.dungeon.room[roomID].floor.SlotsTaken(0, 0)) { ItemMenu(2); valid = !valid; } break;
		case 6: if (run.p.inventory.SlotsTaken(0, 0)) { ItemMenu(1); valid = !valid; } break;
		case 7: return 10;
		case 8:
		case 9: return 99;
		default: run.rend.DropShape(); break; // Need this to prevent overloading shape buffer with looped drawing
		}
	}
	return 0;
}

int Event::Empty() {
	std::string description = "Before you lay a large open tent, walls awash with vibrant colour and posters of the fantastical and horrific, promising both wonder and suffering for all who attend! As you step in the center of the tent you see ";
	return GenericRoom(description);
}

int Event::EntryHall() {
	run.rend.DrawText(newLine("Here you stand, just inside the entrance to the great and terrifying Phantasmagoria, a place of nightmares and death to which you were dragged against your will for the entertainment of the lurking shadows and the vile creature standing beside the locked wooden gate.", 74) + newLine(" ", 60) + newLine("Though a man by first appearance, his entire demeanour reaks of unnatural and unadulterated evil. Even standing still, lent against the fabric side wall, hands on his cane, he seems to move slimely, an anticipatory smile laying across his thin, discoloured lips. His split colour, purple and black tuxedo suits the twisted, bright and discordant nature of the fair, his glistening silver hair slicked back beneath a crookedly worn top hat, a scarlet card slotted in the purple band.",74));
	int sel = 0;
	int valid = 0;
	while (!valid) {
		if (run.rend.DrawPrompt(newLine("[North] [Man]", 56) + "[Character] [Exit] Select an option above marked with []: ")) return -1;
		int sel = run.parse.Selection("north man character exit");
		switch (sel) {
			case 0: run.p.Location(1, 1); valid = !valid; break;
			case 1: return run.eventHandler.Commentator();
			case 2: return 10;
			case 3: return 99;
		default: run.rend.DropShape(); break; // Need this to prevent overloading shape buffer with looped drawing
		}
	}
	return 0;
}

int Event::Carriage() {
	std::string description = "Past an ornate wooden door, you find yourself in a large carriage of some sort, a solid wood construction, with locked doors across from you and at either end of the carriage. Filling the carriage are countless costumes of every sort, ";
	return GenericRoom(description);
}

int Event::Stage() {
	std::string description = newLine("The Room before you towers above the others you've encountered, a great top of such dizzying height that you almost fall over trying to see the top, disappearing off into the darkness above, the room lit only by brazers surrounding the central stage. On said stage is the first person you've seen beside that disturbing man at the entrance. Their figure is sleek, thin and lithe, dressed in a striped wastecoat and pressed trousers. On their face is a masquerade mask, hiding most of their face save their thin grey lips, which wear a wide, sharp toothed grin. In their hand is a whip, but they seem otherwise unarmed. Then a piercing roar fills the room and you are slammed to one side.",72);
	int index = 0;
	int armour = 0;
	int valid = 0;
	while (index < run.p.inventory.SlotsTaken(0, 0) && (run.p.inventory.GetItem(index) != ARMOUR && run.p.inventory.GetItem(index) != CLUB)) index++;
	if (index == run.p.inventory.SlotsTaken(0, 0)) { // If Item not found...
		description += newLine(" ", 70) + newLine("You scream as huge jaws embed themselves in your arm, locking solidly and almost tearing your arm from it's socket. You're pulled half upright then flung across the stage as the jaws release. (Health -40)", 72);
		run.p.Health(2, -40);
		if (run.p.Health(0, 0) <= 0) {
			description += newLine(" ", 70) + newLine("Your skull cracks on the hard surface and your muscles go slack as you slump on the floor, paralysed and unable even to scream as the great beast that attacked you pounces and begins tearing chunks from your body. Your torment is severe but mercifully short as you die of shock as the beast digs into it's meal...", 72);
			run.rend.DrawText(description);
			run.rend.DrawPrompt("Press Enter to Accept Your Fate...");
			run.parse.Enter();
			return Death();
		}
		description += newLine(" ", 70) + newLine("You bounce painfully across the stage, landing face down. Pain lances through your arm, keeping you awake", 72);
		while (!valid) {
			if (defaultMenu()) return -1;
			run.rend.DrawText(description);
			run.rend.DrawPrompt(newLine("[Up] [Roll] [Stay]",72) + "Select an Action Listed Above with []: ");
			int sel = run.parse.Selection("up roll stay");
			run.rend.ClearScreen();
			if (defaultMenu()) return -1;
			switch (sel) {
			case 0:
				run.rend.DrawText("You hastily try to push yourself upright, only eliciting a cry of pain as your bitten arm crumples under you. Then your vision going black as the beast pounces again. It lands on your crumpled form and you feel your body break under it's colossal weight before you feel jaws close almost mercifully around your neck, the world snapping out like a light...");
				run.rend.DrawPrompt("Press Enter to Accept Your Fate...");
				run.parse.Enter();
				return Death();
			case 1:
				run.rend.DrawText("You throw yourself painfully sideways, rolling away just in time to avoid the second pounce of the beast where you just lay, the sound of wood splintering filling the stage as it slams into the wooden deck. With a moment's respite and the beast distracted by flying splinters, you have a chance to run.");
				run.rend.DrawPrompt(newLine("[West] [South] [East]", 72) + "Select an Action Listed Above with []: ");
				switch (run.parse.Selection("west south east")) {
					case 0: run.p.Location(1, run.dungeon.room[run.p.Location(0, 0)].Direction('W', 0, 0)); break;
					case 1: run.p.Location(1, run.dungeon.room[run.p.Location(0, 0)].Direction('S', 0, 0)); break;
					case 2: run.p.Location(1, run.dungeon.room[run.p.Location(0, 0)].Direction('E', 0, 0)); break;
					default: return -1;
				}
				return 0;
			case 2: 
				run.rend.DrawText(newLine("You freeze, not daring to move, barely daring to breath, even in your winded state. You feel as much as hear the sound of the beast's leap up onto the stage, splintering wood spraying across your back, with you just barely suppressing the urge to run. A brush of fur on the back of your neck sends your heart into overdrive, but it disappears after a few fearful seconds.", 72) + newLine(" ", 72) + "You hear the beast pad away towards it's trainer and you take your chance, sprinting for the exit, already on your way before the trainer can shout an order to pursue.");
				run.rend.DrawPrompt(newLine("[West] [South] [East]", 72) + "Select an Action Listed Above with []: ");
				switch (run.parse.Selection("west south east")) {
					case 0: run.p.Location(1, run.dungeon.room[run.p.Location(0, 0)].Direction('W', 0, 0)); break;
					case 1: run.p.Location(1, run.dungeon.room[run.p.Location(0, 0)].Direction('S', 0, 0)); break;
					case 2: run.p.Location(1, run.dungeon.room[run.p.Location(0, 0)].Direction('E', 0, 0)); break;
					default: return -1;
				}
				return 0;
			default: break;
			}
		}
	}
	else if (run.p.inventory.GetItem(index) == ARMOUR) {
		description += newLine(" ", 72) + newLine("You feel something puncture the armour on your wrist as your arm is yanked sideways, before your body hits the floor. The armour does little to soften the impact, but at least you don't crack your skull. You feel a great weight slam down with you, spraying the area with splinters. The jaws enclosing your wrist guard tighten and you feel the teeth sink into the material, grazing your arm.", 72);
		run.rend.DrawText(description);
		run.rend.DrawPrompt(newLine("[Fight] [Flee]", 72) + "Select an Action Listed Above with []: ");
		int sel = run.parse.Selection("fight flee");
		run.rend.ClearScreen();
		if (defaultMenu()) return -1;
		if (!sel) {
			run.rend.DrawText("With a hungry growl, the beast slams a paw against your chest and with a violent scream from yourself, you are conscious just long enough to see your arm torn from it's socket. Then the world goes black.");
			run.rend.DrawPrompt("Press Enter to Accept Your Fate...");
			run.parse.Enter();
			return Death();
		}
		run.rend.DrawText(newLine("Franticly you pull at the straps on your shoulder and with a clatter they come free. The armour's arm is forcably pulled up into the air victoriously, with your arm thankfully no longer inside it.", 72) + newLine(" ", 72) + "Obviously not expecting it's meal to survive such a violent \"injury\", it steps off of you and sits down to knaw into it's metallic meal. Using this distraction, you roll yourself over and kick off into a sprint for one of the exits, moving too quickly for the trainer to do more than shout furiously. With the arm lost and the chestpiece crumpled however, you're certain this won't work a second time");
		run.p.inventory.RemoveItem(index);
		run.rend.DrawPrompt(newLine("[West] [South] [East]", 72) + "Select an Action Listed Above with []: ");
		switch (run.parse.Selection("west south east")) {
			case 0: run.p.Location(1, run.dungeon.room[run.p.Location(0, 0)].Direction('W', 0, 0)); break;
			case 1: run.p.Location(1, run.dungeon.room[run.p.Location(0, 0)].Direction('S', 0, 0)); break;
			case 2: run.p.Location(1, run.dungeon.room[run.p.Location(0, 0)].Direction('E', 0, 0)); break;
		default: return -1;
		}
		return 0;
	}
	else if (run.p.inventory.GetItem(index) == CLUB) {
		description += newLine(" ", 72) + "As you slam into the floor, you manage to keep up your momentum, rolling away as the beast slams down beside you. Using the momentum you've gained, you swing up the Neanderthal's Club and hit the beast in the underside of the jaw, making contact with a sickening crack. The beast staggers for a moment, then collapses, blood soaking its chin. It's far from dead, but it's certainly not waking up any time soon. You could always finish the job, of course, it did try to make you it's lunch.";
		run.rend.DrawText(description);
		run.rend.DrawPrompt(newLine("[Kill] [Spare]", 72) + "Select an Action Listed Above with[]: ");
			switch (run.parse.Selection("kill spare")) {
			case 0: 
				description = "Heaving the club above your head, you then throw all your weight into bringing down the great stone club down of the beast's skull. The room echoes with a hollow crack, followed immediately by a stomach churningly wet impact and a splash of gore across yourself and the surrounding area. The beast's limbs twitch, then fall still, it's most definitely dead."; 
				run.dungeon.room[run.p.Location(0, 0)].Event(1, STAGEEXECUTED);
				break;
			case 1: 
				description = "Pulling yourself to your feet weakly, you stumble back from the beast, giving it a wide berth even in its incapacitated condition.";
				run.dungeon.room[run.p.Location(0, 0)].Event(1, STAGESPARED);
				break;
			default: return -1;
		}
		run.rend.ClearScreen();
		if (defaultMenu()) return -1;
		description += newLine(" ", 72) + "You spin around at the sound of clattering footsteps, only to see the presumed beast tamer fleeing from the stage. You try to give chase, but in your shaken condition, you inevitably fall behind as they slip round a corner of the stands and you hear the heavy thud of a door slamming and the scrape of a bolt sliding into place. As expected, you find nothing but a locked door waiting for you as you turn the corner, too sturdy to break down by hand. And so you're left alone in the room.";
		run.rend.DrawText(description);
		run.rend.DrawPrompt("Press Enter to Continue...");
		run.parse.Enter();
		return 0;
	}
	return -1;
}

int Event::StageExecuted() {
	std::string description = "The Room before you towers above the others you've encountered, a great top of such dizzying height that you almost fall over trying to see the top, disappearing off into the darkness above, the room lit only by brazers surrounding the central stage. Heaped on the ground beside the stage is the bloodied corpse of the beast that attacked you, looking quite disturbing in the silence that fills the room. As it stands ";
	return GenericRoom(description);
}

int Event::StageSpared() {
	std::string description = "The Room before you towers above the others you've encountered, a great top of such dizzying height that you almost fall over trying to see the top, disappearing off into the darkness above, the room lit only by brazers surrounding the central stage. Heaped on the ground beside the stage is the barely breathing form of the beast that attacked you. Even incapacitated you have no desire to go anywhere near it, lest it still feel up for a little revenge. As it stands ";
	return GenericRoom(description);
}

int Event::BackStage() {
	std::string description = "You're now standing in a backstage area, it's full of broken props and torn costumes, but otherwise rather empty. Even ";
	return GenericRoom(description);
}

int Event::Cave() {
	std::string description = newLine("Suddenly the wood and fabric walls give way to rough grey stone all around you, the ceiling dipping low as the room curves roughly around into an alcove of stone. In the middle of the seemingly empty cave, the warm glow of a campfire flickers invitingly.", 70)
							+ newLine(" ", 72)
							+ newLine("As the cold of the stone already starts to seep into your bones, you find yourself drawn to the flame. Only to jump in alarm as an angry roar erupts from just behind you. You scramble away to the other side of the fire, turning to see a great, hulking male, garbed in only fur and body hair.", 72)
							+ newLine(" ", 72)
							+ "His brow looks furrowed until you realise it in fact juts significantly further forward than it should. In his hand is a vicious looking club, and he's waving it in a decidedly threatening manner. Clearly this Neanderthal has no intention of letting you leave his cave alive";
	run.rend.DrawText(description);
	run.rend.DrawPrompt(newLine("[Fight] [Tackle] [Evade]", 72) + "Select an Action Listed Above with []: ");
	int sel = run.parse.Selection("fight tackle evade");
	run.rend.ClearScreen();
	if (defaultMenu()) return -1;
	switch (sel) {
		case 0:
			run.rend.DrawText("You bring your fists to bear, you sprint around the campfire, aiming low for maximum impact, but with your focus shifted to the hulk's largely unprotected groin, it's quickly too late to see the Neanderthal swinging his club unflinchingly right at your head. The blow collides with a sickening crunch, bile rising in your throat as the blow sweeps your numb body up into the air and dumps you directly into the flames of the campfire. Even as a sharp log pierces your sternum and the flames eagerly engulf your ragged form, you feel nothing, for you're already dead...");
			run.rend.DrawPrompt("Press Enter to Accept Your Fate...");
			run.parse.Enter();
			return Death();
		case 1:
			run.rend.DrawText("You stand your ground and with a violent cry, the great goliath hurls himself at you at a dead run. He launches himself over the flames, not giving you any chance to run. But then, you didn't intend to. Right as he lands, you throw yourself at him, slamming your weight against his off-balance form. With a cry, he tumbles backward onto the flames and jerks as a burning wooden stake stabs clean through his sternum. He screams and yells almost endlessly, forcing you to cover your ears to block out the wailing until the flames finally take him. His club drops off to one side and he emits his last gurgling whine. He is most definately dead.");
			run.rend.DrawPrompt("Press Enter to Continue...");
			run.dungeon.room[run.p.Location(0, 0)].Event(1, CAVECLEAR);
			run.parse.Enter();
			return 0;
		case 2:
			run.rend.DrawText(newLine("Feeling it would be unwise to actively engage such a brute, you make to evade, just in time to avoid a swipe of his club as he lands a leap over the flames. He charges after you and despite appearances he can certainly keep up with your ducking and weaving, with his club grazing you harder with each swipe. After getting all the way around the fire to the entrance, you trip on a stone bowl you didn't notice and land on your back, just barely missing a long bone tusk propped upright right next to your head.", 72)
							+ newLine(" ", 72)
							+ "Seeing your chance, you kick out at the Neanderthal's feet. He trips as you dive out of way, so you all you hear is a thud and a blood curdlingly wet sound as he lands. Looking back, you see the hulking brute with the spike driven up through his gaping mouth and up through his skull, the exposed tip of the bone spike glistening with blood and gore. Resisting the urge to throw up, you push his still twitching form into the fire, his huge form engulfed in flame in a few moments, his furs crashling as they burn.");
			run.rend.DrawPrompt("Press Enter to Continue...");
			run.dungeon.room[run.p.Location(0, 0)].Event(1, CAVECLEAR);
			run.parse.Enter();
			return 0;
		default: return -1;
	}
}

int Event::CaveClear() {
	std::string description = "In this cavern of grey, coarse stone, a small fire crackles it's last around the Neanderthal's charred corpse. Around the fire is little of interest, ";
	return GenericRoom(description);
}

int Event::Den() {
	std::string description = newLine("As you step into the room, your eyes drink in the sight what appears to be part lavish parlor, part torturer's dungeon. A bar sits at the far end with no-one to man it, human bones sit blackening in locked cages over a roaring fire, while to your side you see a table said out for playing dice. Beyond the table is a most disturbing sight. A man, sat rigid in a spiked metal contraption, spindly arms holding open his reddened, wild eyes, a crazed smile on his lips even as the contraption pierces his flesh all across his body.", 72) + newLine(" ", 72) + "\"Good Evening...\" He croaks, in a twisted, slathering voice, \"Care to try you hand at a little game to pass the time?\" He asks, his wild eyes unblinkingly locked with your own.";
	run.rend.DrawText(description);
	run.rend.DrawPrompt(newLine("[Accept] [Refuse]", 72) + "Select an Action Listed Above With []: ");
	int sel = run.parse.Selection("accept refuse");
	run.rend.ClearScreen();
	if (defaultMenu()) return -1;
	if (sel) {
		run.rend.DrawText("\"Then GET OUT!!\" He screaches, fidgetting madly, fresh blood dripping from the wounds where he is impaled by his chair as he cackles madly \"Want to play a game? Want to play a game? HAHAHAHA!\" As he screams his delight, you feel an unseen force grab you tightly and throw you from the room, the door slamming shut behind you. (Health -10)");
		run.p.Location(1, 9);
		run.p.Health(0, -10);
		run.rend.DrawPrompt("Press Enter to Continue...");
		return 0;
	}
	run.rend.DrawText(newLine("With your agreement, he screaches with delight, a mad, cackling howl of delight... Before you can react, spikes embed themselves in your flesh and as you cry out you find yourself in a contraption akin to the madman across from you. At first all you can think about is the pain, but as it gives way to a dull throb you realise the madman is explaining something", 72)
		+ newLine(" ", 72)
		+ "\"...you'll decide the game, my dear challenger! Hahaha! Pick your poison! So long as we use Dice! I've never lost a bet, the spikes reward me greatly for victory... The spikes... Oh the spikes... The pain is so good...\" His trails off, looking almost longingly at the contraption that imprisons him. As disturbed and in pain as you are, you struggle to focus and try to pick a game, while he caresses the thorny dice piercing his hand.");
	run.rend.DrawPrompt(newLine("[Lucky] Dice  Sly [Bet]  [High] Dice",72) + "Select an Action Listed Above With []: ");
	sel = run.parse.Selection("lucky bet high");
	run.rend.ClearScreen();
	if (defaultMenu()) return -1;
	switch (sel) {
	case 0:
	case 2:
		if (!sel) { description = newLine("The Gambler giggles in delight. \"Oh Lucky Dice, Lucky Dice! I love this game!\" He cries, fidgetting in glee in his chair, the spikes digging deeper into his flesh. He throws his spiked dice into the air and they seem to blur and split, multiplying in a disturbing double image to form maybe half a dozen dice that clatter down onto the table. With a vicious, certain grin, the Gambler declares \"Let's Play!\"...", 72); }
		else { description = "The Gambler laughs hysterically. \",~ High Dice! High Dice! Test your luck with High Dice! ,~\" He signs, bouncing in his mechanical prison, flesh streams of blood dripping down his face and arms. Then he looks momentarily sad \"Too bad... You never had a chance,\"..."; }
		run.rend.DrawText(description + newLine(" ", 72) + "You lost... Again and again, you lost, every roll trumped by the gambler's own flawless, impossible rolls, but you can't stop playing... Your mind is full of fog, pain and the sound of the Gambler's mad howls of delight, the spikes digging deeper into your flesh with every loss, pumping something into you that makes it hard to think. After every loss, the gambler forces the spiked dice into your hands, tearing the flesh as you're forced to roll again and again. So lost in the fog and pain, you don't even notice when you finally die...");
		run.rend.DrawPrompt("Press Enter to Accept Your Fate...");
		run.parse.Enter();
		return Death();
	case 1:
		description = newLine("The Gambler's eyes narrow at your thoughtful tone, \"I'll take any bet, never lost, never will\" He says sourly. So you make your proposal, you bet he can't deliberately lose a game. The Gambler's eyes widen in horror \"No! No! NOT FAIR! NOT FAIR! NOT FAIR!\" He screams, thrashing in his mechanical cage, tearing his own flesh in his frantic rage. But his hands raise, as if guided by his spikey prison, and with a cry of pain, he's forced to roll the dice. With a whimper he breaths out \"High Dice...\"", 72);
		run.rend.DrawText(description + newLine(" ", 72) + newLine("You won, at least in the end... Again and again, you lost, every roll trumped by the gambler's own flawless, impossible rolls, but the nature of your bet forced the spikes of the his chair deeper into his withered flesh. He'd scream and beg with every roll, but still got perfect results without fail. Every time he lost, you were pumped full of chemicals, your mind awash in an artificial high. This went on for what seemed like an eternity and you understand at last what drove the Gambler mad.", 72));
		run.rend.DrawPrompt("Press Enter to Continue...");
		run.parse.Enter();
		run.rend.ClearScreen();
		if (defaultMenu()) return -1;
		run.rend.DrawText("But finally, rather more regretfully than you'd like to admit, the high finally fades, and you focus at last to see the Gambler hanging limply in his mechanical prison, his flesh torn and ravaged, the pool of blood around him reaching your own feet. Rather reluctantly, you start pulling away from your own prison, and to your great surprise, it releases you easily. You can only imagine it's the addiction to that beautiful, wonderful high... You snap back into focus to find your hand has drifted longingly towards the machine. You snatch in back and move away, distracting yourself by inspecting the room arround you.");
		run.dungeon.room[run.p.Location(0, 0)].Event(1, DENCLEAR);
		run.rend.DrawPrompt("Press Enter to Continue...");
		run.parse.Enter();
		return 0;
		default: break;
	}
	return -1;
}

int Event::DenClear() {
	std::string description = newLine("Looking aboput the room, your eyes drink in the sight what appears to be part lavish parlor, part torturer's dungeon. A bar sits at the far end with no-one to man it, human bones sit blackening in locked cages over a roaring fire, while to your side you see a table said out for playing dice. Beyond the table is a most disturbing sight. A man's corpse, sat rigid in a spiked metal contraption, spindly arms holding open his reddened, rolled back eyes, his face drawn back in a fixed look of pain and horror, blood dripping from the body where the contraption pierces his flesh all across his body, the pool of blood congealing beneath him in the warm air. The maddened Gambler you killed so you could survive...", 72) + newLine(" ", 72) + "Across from the body is the contraption that imprisoned you and did it's best to force on you a sweet, beautiful... an addiction that would keep you imprisoned by your own will. With a shudder, you turn away, looking for anything else of note. But ";
	return GenericRoom(description);
}

int Event::Workshop() {
	std::string description = "You enter the room through an ornate wooden door to find a room filled with a huge collection of magician's paraphernalia. In one corner is a dressing table with an array of perfumes and makeup. Either the magician's or the assistant's, you can't tell, though you wonder why you're even wondering that in this life or death situation. Maybe you're already getting a little desensitised to it all. Or maybe you're just tired... Regardless, looking around, most of the props are of little real use to you, certainly nothing that catches your eye, meanwhile ";
	return GenericRoom(description);
}

int Event::CubbyHole() {
	std::string description = "At first you think you've reached a dead end, then you notice a little hatch low on the wall. Opening it up, you find a tiny crawl space, which someone has converted into a little cubby hole or nest. Despite the strange nature of the place it's located, it looks strangely cozy and inviting. ";
	int roomID = run.p.Location(0, 0);
	int index = 0;
	while (index < run.dungeon.room[roomID].floor.SlotsTaken(0, 0) && run.dungeon.room[roomID].floor.GetItem(index) != run.dungeon.room[roomID].floor.GetItem(index)) index++;
	if (run.dungeon.room[roomID].floor.GetItem(index) == BLANKET) description += "A thick blanket covers a crude mat of straw and yet more straw forms a rough pillow. ";
	else description += "Filling the space is a crude mat of straw, with a bigger pile of straw forming what could generously be called a pillow. ";
	description += "You could certainly use some rest after this ordeal, but you're not sure you should risk it... ";
	if (!run.TicketTaken(0, 0)) {
		if (run.dungeon.room[roomID].floor.GetItem(index) == BLANKET) description = newLine(description, 72) + newLine(" ", 72) + "Thats when you notice what's sitting on top of the blanket. ";
		else description += "That's when you notice something on the bed, half covered in loose hay. ";
		description += "A black piece of paper, surface covered in an intricate silver design, you can't help but wonder who it belonged to. Apparently not one of the living, if the dark stain on the torn corner is anything to go by. Looking closer however, you realise it must be the ticket you were forcefully tasked with finding. So you've found the ticket, but what now ? ";
	}
	else description += "So... What now?";
	int valid = 0;
	while (!valid) {
		run.rend.ClearScreen();
		if (defaultMenu()) return -1;
		run.rend.DrawText(description);
		run.rend.DrawNavigationPrompt();
		int sel = run.parse.Selection("north west south east take swap drop character exit");
		switch (sel) {
		case 0: if (run.dungeon.room[roomID].Direction('N', 0, 0) != -1) { run.p.Location(1, run.dungeon.room[roomID].Direction('N', 0, 0)); valid = !valid; } break;
		case 1: if (run.dungeon.room[roomID].Direction('W', 0, 0) != -1) { run.p.Location(1, run.dungeon.room[roomID].Direction('W', 0, 0)); valid = !valid; } break;
		case 2: if (run.dungeon.room[roomID].Direction('S', 0, 0) != -1) { run.p.Location(1, run.dungeon.room[roomID].Direction('S', 0, 0)); valid = !valid; } break;
		case 3: if (run.dungeon.room[roomID].Direction('E', 0, 0) != -1) { run.p.Location(1, run.dungeon.room[roomID].Direction('E', 0, 0)); valid = !valid; } break;
		case 4: if (run.dungeon.room[roomID].floor.SlotsTaken(0, 0)) { ItemMenu(0); valid = !valid; } break;
		case 5: if (run.p.inventory.SlotsTaken(0, 0) && run.dungeon.room[roomID].floor.SlotsTaken(0, 0)) { ItemMenu(2); valid = !valid; } break;
		case 6: if (run.p.inventory.SlotsTaken(0, 0)) { ItemMenu(1); valid = !valid; } break;
		case 7: return 10;
		case 8:
		case 9: return 99;
		default: run.rend.DropShape(); break; // Need this to prevent overloading shape buffer with looped drawing
		}
	}
	return 0;

}

int Event::Tormentor() {
	run.rend.DrawText(newLine("And you hit the ground, face first into a hard, cracked surface coated in a thin layer of dirt and dust which puffs out around you as you hit it. Pushing yourself upright, you look around to see you've entered a colossal oval pit, the walls covered in rusted metal spikes and at the far end of the pit is an equally huge set of doors.", 72)
					+ newLine(" ", 72)
					+ newLine("As you look further up the door, something at the top of the pit catches your eye and you can't help but recoil in horror at the sight of a literal horde of hellish figures, beasts and monsters. Every form you can imagine and everything else besides was up there. Judging by their eyes, those that have eyes at least, they're watching the far end of the oval, just behind you.", 72)
					+ newLine(" ", 74)
					+ "You spin around and get a waft of foul breath and sulphur, forcing you to back up with in a coughing fit. Towering above you is a huge, barechested man, muscles overblown to the point that they look ready to burst. He wears a black executioner's hood and thick, rough leather trousers. He's like an invincible wall that's about to topple and crush you...");
	run.rend.DrawPrompt("Press Enter to Continue...");
	run.parse.Enter();
	run.rend.ClearScreen();
	if (defaultMenu()) return -1;
	run.rend.DrawText(newLine("~ CHOOSE YOUR DEATH ~ The words echo in your mind and you hear an excited roar from the swarm of hellions around the pit, the shouting hurting your ears it's so loud. After a long moment, the giant repeats himself more forcably, making your head ring with his telepathic command.", 72)
					+ newLine(" ", 72)
					+ "Suddenly you feel a sharp tug at your clothing and the items you had with you pull themselves from where they were stored and array themselves before you. ~ CHOOSE YOUR DEATH ~ He repeats one more time, red beady eyes glaring down at you impatiently...");
	std::string prompt = "[Refuse]";
	for (int i = 0; i < run.p.inventory.SlotsTaken(0,0); i++) {
		prompt += " [" + run.itemList[run.p.inventory.GetItem(i)].Name(0, "") + "]";
	}
	run.rend.DrawPrompt(newLine(prompt,72) + "Select an Item Listed Above With []: ");
	int sel = run.parse.Selection("refuse dice club hat armour blanket");
	std::string description;
	switch (sel) {
	case 0:
		description = "You stand your ground, folding your arms defiantly, staring right back at the goliath before you. Obviously disappointed, he sighs and waves a hand and in confusion you look down to see your lower half disappear into mist, followed by darkness as what's left of you is crushed in the Goliath's mental grip";
		break;
	case 1:
		description = "As you point reluctantly to the dice floating before you, the Goliath stares intently at them ~No More Games With The Little Man~ The voice in your head muses, almost sadly.But that's only for a moment. Immediately afterwards, his eyes focus back on you with a fierce and heartless intensity. The other items fall to the ground and the dice approach the Goliath, blurring and splitting into a great deal more, until it forms a small cloud of metal and spikes. After only a moment, the cloud swarms towards you, engulfing you in it's deadly embrace, tearing, piercing and shreding your skin, bone and organs with ease.You can do naught but scream as it tears you apart piece by piece until it finally deals a fatal blow and the world finally goes dark.";
		break;
	case 2:
		description = "The Goliath seems to nod approvingly as you point the the large stone club floating before you and gives it a violent swing. There's a bloody crunch and you blink as you realise you've been thrown to one side, you left side feeling strangely numb until you look over and stare at the bloody, torn stump that's left. With a thud, the rest of your arm lands beside you as the pain starts to register with your brain, your throat closing too tight to even scream. The goliath takes his time destroying each of your limbs in turn, your brain in too much of a state of shock to even react. He seems almost disappointed as you slip into the warm embrace of death...";
		break;
	case 3:
		description = "The Goliath stares at the Magician's hat blankly as you point at it, before floating it over to himself and taking it into his hands. Almost jovially, he tries it on, popping it on top of his hood with a flick of the wrist. A wrist that promptly disappears if a flash of light, along with the Goliath's head and most of his torso, having vanished in an instant. It seems the hat didn't have enough magic left to take all of his colossal form, so simply tore a man sized hole from the space below where the hat had been placed. Presumably the counterpart to the bloody remains folding in on themselves have appeared in the Entry Hall, but maybe it just vanished into the eather... You can only stand there stunned as the whole arena goes silent.";
		run.rend.ClearScreen();
		if (defaultMenu()) return -1;
		run.rend.DrawText(description);
		run.rend.DrawPrompt("Press Enter to Continue...");
		run.parse.Enter();
		return Victory();
	case 4:
		description = "As soon as you point to the armour you're wearing, you kick out wildly as you start being lifted into the air. After being lifted up several meters into the air, the grip of you tightens and you feel the armour almost immediately starts to buckle. The first thing to be crushed is your arms, eliciting a despirate scream from you, only to be cut off as your legs go the same way, overwhelming you with the pain to the point your throat locks up. Thankfully, the crushing pressure on your armoured skull quickly relieves you of the pain as the helmet buckles and the world goes black...";
		break;
	case 5:
		description = newLine("You point to the bundle of cloth floating before you and the Goliath sets his focus on it. After a long moment nothing really happens, the fabric barely moving, as if to start untangling itself, but it's packed too tight, so it's loose section just flaps weakly. With a growl, the Goliath reaches out and snatches the bundle from the air and begins to unwrap it by hand.", 72) + newLine(" ", 72);
		switch (run.HiddenItem(0, 0)) {
		case BONE:
			description += newLine("As soon as the bone is exposed to open air, you feel that familiar chill of death run through you. The Goliath twitches violently, a faint gurgle coming from under his hood as he stares at the acursed spike. But it's already too late, he's gotten a grip of it while he was unwrapping the blanket. Over the roar of the crowd, you honestly think you hear a strained whimper from the Goliath.", 72);
			run.rend.ClearScreen();
			if (defaultMenu()) return -1;
			run.rend.DrawText(description);
			run.rend.DrawPrompt("Press Enter to Continue...");
			run.parse.Enter();
			description = "Ever so slowly, his muscles straining in defiance, he raises the bone spike to his throat, his hands shaking. Then, ever so slowly, he start carving deeply into his throat, his whimpering disolving into gurgles as blood wells up from the wounds, running in thick streams down his chest. He continues carving away, body shaking endlessly, leaving you to watch silently in quiet horror. Even the horde around you has gone quiet, you can feel a million inhuman eyes directed at you and the Goliath. After what seems like an eternity, the Goliath stops gouging and braces his hands at the base of the spike. With a colossal heave, he rams in straight up, into and through his hooded skull, immediately slumping to the ground, dead.";
			run.rend.ClearScreen();
			if (defaultMenu()) return -1;
			run.rend.DrawText(description);
			run.rend.DrawPrompt("Press Enter to Continue...");
			run.parse.Enter();
			return Victory();
		case DICE:
			description += newLine("As the Dice fall to the ground, the Goliath stares intently at them ~ No More Games With The Little Man~ The voice in your head muses, almost sadly. But that's only for a moment. Immediately afterwards, his eyes focus back on you with a fierce and heartless intensity. The other items fall to the ground and the dice approach the Goliath, blurring and splitting into a great deal more, until it forms a small cloud of metal and spikes. After only a moment, the cloud swarms towards you, engulfing you in it's deadly embrace, tearing, piercing and shreding your skin, bone and organs with ease. You can do naught but scream as it tears you apart piece by piece until it finally deals a fatal blow and the world finally goes dark.", 72);
			break;
		case CLUB:
			description += newLine("The club falls from the blanket with a heavy THUD, bouncing briefly before being pulled up in the Goliath's mental grip. The Goliath seems to nod approvingly and gives it a violent swing. There's a bloody crunch and you blink as you realise you've been thrown to one side, you left side feeling strangely numb until you look over and stare at the bloody, torn stump that's left. With a thud, the rest of your arm lands beside you as the pain starts to register with your brain, your throat closing too tight to even scream.", 72)
				+ newLine(" ", 72)
				+ "The goliath takes his time destroying each of your limbs in turn, your brain in too much of a state of shock to even react. He seems almost disappointed as you slip into the warm embrace of death...";
			break;
		case HAT:
			description += newLine("The Magicians's Hat falls to the ground silently and the Goliath stares at it blankly before floating it over to himself and taking it into his hands. Almost jovially, he tries it on, popping it on top of his hood with a flick of the wrist. A wrist that promptly disappears if a flash of light, along with the Goliath's head and most of his torso, having vanished in an instant. It seems the hat didn't have enough magic left to take all of his colossal form, so simply tore a man sized hole from the space below where the hat had been placed. Presumably the counterpart to the bloody remains folding in on themselves have appeared in the Entry Hall, but maybe it just vanished into the eather... You can only stand there stunned as the whole arena goes silent.", 72);
			run.rend.ClearScreen();
			if (defaultMenu()) return -1;
			run.rend.DrawText(description);
			run.rend.DrawPrompt("Press Enter to Continue...");
			run.parse.Enter();
			return Victory();
		case ARMOUR:
			description += "The heavy pieces of armour fall to the ground with a crash. After a silent moment, the armour raises itself up and floats over to you, strapping itself into place over your ragged clothes. Once fully armoured, you kick out wildly as you start being lifted into the air. After being lifted up several meters into the air, the grip of you tightens and you feel the armour almost immediately starts to buckle. The first thing to be crushed is your arms, eliciting a despirate scream from you, only to be cut off as your legs go the same way, overwhelming you with the pain to the point your throat locks up. Thankfully, the crushing pressure on your armoured skull quickly relieves you of the pain as the helmet buckles and the world goes black...";
			break;
		default:
			description += "The bundle unfurls to reveal... nothing. Obviously disappointed, the Goliath wastes no time in turning his focus onto you, and the blanket lunges forwards like a snake, in the blink of an eye wrapping you in it like a constrictor. Your chest in immediately under strain as it begins squeezing you tightly. But it takes it's time crushing the life out of you. To keep you from passing out it occasionally reaches out and wraps around and then crushes a limb. This goes on for what seems like a breathless eternity before the Goliath apparently gets bored and wraps it around your head, the world going black in an instant as it crushes your chest and skull in one big grand finale...";
			break;
		}
		break;
	default: return -1;
	}
	run.rend.ClearScreen();
	if (defaultMenu()) return -1;
	run.rend.DrawText(description);
	run.rend.DrawPrompt("Press Enter to Accept Your Fate...");
	run.parse.Enter();
	return Death();
}

int Event::Club() {
	std::string description = "You stare at the club, wondering who you should use it on...";
	run.rend.DrawText(description);
	std::string prompt = "[Self]";
	std::string selection = "self";
	if (!run.p.Location(0, 0)) { // If in Entrance Hall (Room 0), add the Commentator to the list of potential victims
		prompt += " [Man]";
		selection += " man";
	}
	prompt = newLine(prompt, 68) + "[Back] Select an Action Above Listed With []: ";
	selection += " back";
	run.rend.DrawPrompt(prompt);
	int sel = run.parse.Selection(selection);
	run.rend.ClearScreen();
	if (defaultMenu()) return -1;
	if (!sel) {
		run.rend.DrawText(newLine("You suppose it would be nicer to put yourself out of your misery before you can be a tormented plaything for that man any more than you already have been. You're not getting out alive, you're certain of that now. May as well make it quick.",72) + newLine(" ", 72) + "Mentally preparing yourself, you ready the club and swing up at yourself as hard as you can. The world goes black...");
		run.rend.DrawPrompt("Press Enter to Accept Your Fate...");
		run.parse.Enter();
		return Death();
	}
	else if (sel == 1 && !run.p.Location(0, 0)) {
		run.rend.DrawText(newLine("You approach the man with a dark look of determination... \"Back already are we? Well that's simply no fun at all, be a good victim and go grovel or cry or something,\" The man says dismissively, looking earnestly bored with you now.", 72) + newLine(" ", 74) + newLine("Then he notices the club in your hand and his eyebrows raise in surprise, a hint of approval in his tone as he adds \"I see, you're quite the resourceful one, aren't you?\" Before he can say more, there's a sickening crack as you lash out, his top hat flying across the room as he slumps against the wall. \"Wait! Wait a moment! Let's be reasonable here!\" He declares in alarm, voice slurred and blood trickling down his face.", 72) + newLine(" ", 72) + "Bloodlust grips you as you swipe at him tirelessly, bones cracking, blood spurting, his voice getting weaker and weaker. His stoic, playful demeaner gone, but instead he sobs like a child, gasping and begging between blows for mercy, forgiveness, anything... Finally, in a bloody, misshapen mess, he breaths his last. And as he does, a twisted smile crosses your lips.");
		run.rend.DrawPrompt("Press Enter to Continue...");
		run.parse.Enter();
		return SecretEnding();
	}
	else return 0;
}

int Event::CursedBone() {
	std::string description = "As you reach down to pick up the disturbing bone spike, your hand brushes its surface and a chill runs through your flesh, a stark warning, a brush with danger that sets your blood on fire and your heart beating franticly. You stumble back and wonder if this is a good idea...";
	run.rend.DrawText(description);
	run.rend.DrawPrompt(newLine("[Take] [Leave]", 72) + "Select an Action Listed Above With []: ");
	int sel = run.parse.Selection("take leave");
	run.rend.ClearScreen();
	if (defaultMenu()) return -1;
	if (!sel) {
		run.rend.DrawText(newLine("As you place your hand on the bone's surface, you feel its aura of death wrap around you, choking you, locking you in place. After a long moment, your hand starts tightening around the bone, lifting it up against your will. It slowly comes up, the tip pressing into the base of your neck. Your other hand braces the other end, and with a hard shove, the world goes black, three words lingering in your fading mind...", 72) + newLine(" ", 72) + "~ YOU WERE WARNED ~");
		run.rend.DrawPrompt("Press Enter to Accept Your Fate...");
		run.parse.Enter();
		return Death();
	}
	else return -1; // Fail to take item
}

int Event::Blanket() {
	int roomID = run.p.Location(0, 0);
	if (!run.HiddenItem(0,0)) { // If no item hidden in blanket, select item to hide
		std::string description = newLine("What item do you want to pick up in the blanket?", 72);
		std::string prompt = "";
		std::string selection = "";
		std::string name = "";
		for (int i = 0; i < run.dungeon.room[roomID].floor.SlotsTaken(0, 0); i++) {
			description += newLine(" ", 72) + newLine(run.itemList[run.dungeon.room[roomID].floor.GetItem(i)].Description(0, ""), 72);
			name = run.itemList[run.dungeon.room[roomID].floor.GetItem(i)].Name(0, "");
			prompt += "[" + name + "] ";
			selection += name + " ";
		}
		prompt = newLine(prompt, 68) + "[Exit] Select an Item Listed Above With []: ";
		selection += " exit";
		run.rend.DrawText(description);
		run.rend.DrawPrompt(prompt);
		int sel = run.parse.Selection(selection);
		run.rend.ClearScreen();
		if (defaultMenu()) return -1;
		int item = run.dungeon.room[roomID].floor.GetItem(sel);
		run.rend.DrawText("Leaning down carefully you lay the blanket over the " + run.itemList[item].Name(0, "") + " and wrap it up, rolling it over and folding the blanket into a tight bundle.");
		if (run.dungeon.room[roomID].floor.RemoveItem(sel)) return -1;
		run.HiddenItem(1, item);
		run.rend.DrawPrompt("Press Enter to Continue...");
		run.parse.Enter();
		return 0;
	}
	else { // Otherwise remove item from blanket and drop on the floor
		run.rend.DrawText("You tenderly take the bundled blanket and lay to out on the floor, unwrapping the " + run.itemList[run.HiddenItem(0,0)].Name(0,"") + " and pulling the blanket from under it, rolling it out onto the floor.");
		if (run.dungeon.room[roomID].floor.AddItem(run.HiddenItem(0, 0))) return -1;
		run.HiddenItem(1, 0);
		run.rend.DrawPrompt("Press Enter to Continue...");
		run.parse.Enter();
	}
	return 0;
}

int Event::Ticket() {
	if (run.TicketTaken(0, 0)) return 0; // If flag already set, exit cleanly
	run.TicketTaken(1, -1); // Otherwise set flag
	return 0;
}

int Event::Commentator() {
	run.rend.ClearScreen();
	if (defaultMenu()) return -1;
	int index = 0;
	while (index < run.p.inventory.SlotsTaken(0, 0) && run.p.inventory.GetItem(index) != TICKET) index++;
	if (run.p.inventory.GetItem(index) == TICKET) { // If you have the ticket, begin endgame
		run.rend.DrawText(newLine("\"You found it! Haha! Congratulations my dear guest, for your efforts you'll recieve a special reward!", 70) + newLine(" ", 74) + newLine("\"What was that? You want to go home?! But the games have only begun! We can't let you go before the main event now can we? Now stop that, shouting will make no difference, you will be the guest of honour at the event of the century!\"", 72) + newLine(" ", 70) + newLine("With a snap of his fingers, you feel suddenly nausious as the wall to the west distorts and folds open unnaturally. Before you can object, the commentator snatches the ticket and reaches a hand behind you, pushing you through the opening...", 74));
		run.p.inventory.RemoveItem(index);
		run.rend.DrawPrompt("Press Enter to Accept Your Fate...");
		run.parse.Enter();
		run.p.Location(1, 17);
		return 0;
	}
	else { // Else enter standard dialog with Commentator
		run.rend.DrawText(newLine("So, I see you're awake at last. I do so hope you'll enjoy your... brief time that you'll be with us, we most certainly will.", 72) + newLine(" ", 74) + "But of course, I'm sure you have questions...");
		int sel = -1;
		while (sel != 4) {
			run.rend.DrawPrompt(newLine("[Who] [Why] [Goal] [Attack]", 68) + "[Exit] Select an option above marked with []: ");
			sel = run.parse.Selection("who why goal attack exit");
			run.rend.ClearScreen(); // Clears screen and shape/text buffer inside loop, preventing overflow
			if (defaultMenu()) return -1;
			switch (sel) {
			case 0: run.rend.DrawText(newLine("Why, I am a simple showman responsible for hurding you innocent souls to our little play pen. If you mean who we are, we're a travelling hellion troupe of showman and performers, on our grand tour from Hell to the fourteen dimensions, your little realm is simply the first on our list.", 72) + newLine(" ", 70) + "Was there anything else?"); break;
			case 1: run.rend.DrawText(newLine("Our little troupe puts on lavish performances for the denizens of the fourteen realms. As it turns out, they have a particular love of splendor, colour and torment. Nothing gets the crowds screaming with glee quite like a good bit of bloody murder and slow, painful torture. Ah Memories...", 72) + newLine(" ",70) + "So, was there any other questions?"); break;
			case 2: run.rend.DrawText(newLine("Why our goal here is quite simple. We want you to play a little game with us. All you have to do is find a single ticket hidden somewhere in our humble fair. Find it, get back here alive and you'll be rewarded...", 72) + newLine(" ", 70) + "That clear enough for you?"); break;
			case 3: run.rend.DrawText(newLine("You lunge desperately at the grinning man, who locks up for a moment in wide eyed alarm before deftly sidestepping your swinging fist and slamming his cane into the back of your skull, smacking you into the floor fiercely. (Health -20)", 72) + newLine(" ", 70) + "As the ringing in your ears dies down, you pull yourself weakly to your feet and see the man smiling innocently at you, head cocked expectantly."); run.p.Health(2, -20); break;
			default: break;
			}
		}
		return 0;
	}
}

int Event::Death() {
	run.rend.ClearScreen();
	if (defaultMenu()) return -1;
	run.rend.DrawText("And so you died, cold and alone, lost and forgotten in the fair of Phantasmagoria, never to be found, simply vanishing from the annals of history like a ghost...");
	run.rend.DrawPrompt("Press Enter to Steal Away a New Innocent Soul...");
	run.parse.Enter();
	run.save.Load(-1); // Load Fresh Game
	return 99; // Return to Main Menu
}

int Event::SecretEnding() {
	run.rend.ClearScreen();
	if (defaultMenu()) return -1;
	run.rend.DrawText(newLine("Your tongue flicks out to savour the splattered blood on your lips as you drop the club beside the bloody pulp before you. You turn, taking a step to use side to pick up the crumpled top hat off the floor.", 72) + newLine(" ", 72) + newLine("Brushing it off, you sit it atop your head as your form and clothing shivers and shifts. Your ragged attire becomes a flood of midnight black and garish purple, your face tightens and shifts, your lips thinning into a crooked smile...", 72) + newLine(" ", 72) + "\"Time for the next tender soul to play our little game, this one was quite fun indeed...\" The man mused, leaning back beside gate, a man of viscous, unadulterated evil, born again and eager to play his games...");
	run.rend.DrawPrompt("Press Enter to Steal Away a New Innocent Soul...");
	run.parse.Enter();
	return 99;
}

int Event::Victory() {
	run.rend.ClearScreen();
	if (defaultMenu()) return -1;
	std::string description = newLine("Screams of rage fill the arena, your ears reeling at the unnatural sound. You force your eyes closed and wait for it to end, feeling not entirely satisfied with this small victory, but it'll have to do... Only the finishing blow never comes and as you open your eyes you realise you haven't been hearing screams of rage, but fear, as the horde does it's best to cover and flee from the edge of the pit. At first you're certain something even worse has appeared, but from the look of those mosters that have eyes, all of them are staring right at you.", 72)
		+ newLine(" ", 72)
		+ "You hear a forceful footstep directly behind you and spin to come face to face with the man from the entrance, his calm demeanor shattere and his face drawn back in inhuman rage. But instead of attacking, you find yourself flinching as he throws a small glittering object at you. You manage to catch it and look down to see it's a small key. \"Get... Out,\" The Man growls furiously. Even as you watch, his face is twisted to the limit, eyes burning with unearthly flame. Taking this chance you've been given, you turn and run to the large doors at the end of the arena. There you find a small keyhole, and as you put the key in the hole, the world twists around you and the world goes black...";
	run.rend.DrawText(description);
	run.rend.DrawPrompt("Press Enter to Continue...");
	run.parse.Enter();
	run.rend.ClearScreen();
	if (defaultMenu()) return -1;
	run.rend.DrawText("You wake up in darkness, but as you heave yourself upright, your eyes start to adjust to light of the waxing moon above you. You stand and find yourself on the edge of the river, not far from where you first got snatched up, you believe. It's late and only the waves of the river can be heard. This always was a quiet place, but after your ordeal, the silence in unnerving. You make to leave when you notice the weight in your hand, and find yourself holding the key in a death grip. For a moment you're tempted to throw it in the river, but pocket it after a grim thought. If they come for you again, you may need it, and this time, you'll be ready for them...");
	run.rend.DrawPrompt("Press Enter to Survive...");
	run.parse.Enter();
	return 99;
}

int Event::Sleep() {
	run.rend.DrawText(newLine("With some effort, you manage to squeeze into the tight cubby hole and stow yourself away. Somewhat reluctantly, you pull the hatch closed. It's a tight fit and somewhat claustrophobic, but it's also particularly warm and comfortable, even with a pile of straw for a mattress. You quickly find yourself drifting off to sleep...",72) + newLine(" ", 72) + "When you wake back up, you feel a little drowsy, but a lot better than you did before, perhaps enough to face this place after all. (Health Restored)");
	run.p.Health(1, 100);
	run.rend.DrawPrompt("Press Enter to Continue...");
	run.parse.Enter();
	return 0;
}
