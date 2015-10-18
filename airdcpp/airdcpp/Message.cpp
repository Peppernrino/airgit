/*
 * Copyright (C) 2001-2015 Jacek Sieka, arnetheduck on gmail point com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "stdinc.h"

#include "Message.h"
#include "ClientManager.h"
#include "Magnet.h"
#include "OnlineUser.h"
#include "QueueManager.h"
#include "ResourceManager.h"
#include "ShareManager.h"
#include "Util.h"

namespace dcpp {

uint64_t idCounter = 0;

ChatMessage::ChatMessage(const string& aText, const OnlineUserPtr& aFrom, const OnlineUserPtr& aTo, const OnlineUserPtr& aReplyTo) noexcept :
	text(aText), from(aFrom), to(aTo), replyTo(aReplyTo), id(idCounter++), time(GET_TIME()) {

	read = aFrom && aFrom->getUser() == ClientManager::getInstance()->getMe();
}

LogMessage::LogMessage(const string& aMessage, LogMessage::Severity sev) noexcept : id(idCounter++), text(aMessage), time(GET_TIME()), severity(sev) { }

string ChatMessage::format() const {
	string tmp;

	//if(timestamp) {
	//	tmp += '[' + Util::getShortTimeString(timestamp) + "] ";
	//}

	const string& nick = from->getIdentity().getNick();
	// let's *not* obey the spec here and add a space after the star. :P
	tmp += (thirdPerson ? "* " + nick + ' ' : '<' + nick + "> ") + text;

	// Check all '<' and '[' after newlines as they're probably pastes...
	size_t i = 0;
	while( (i = tmp.find('\n', i)) != string::npos) {
		if(i + 1 < tmp.length()) {
			if(tmp[i+1] == '[' || tmp[i+1] == '<') {
				tmp.insert(i+1, "- ");
				i += 2;
			}
		}
		i++;
	}

	return Text::toDOS(tmp);
}

} // namespace dcpp