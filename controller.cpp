/* Copyright (C) 2013-2014 Michal Brzozowski (rusolis@poczta.fm)

   This file is part of KeeperRL.

   KeeperRL is free software; you can redistribute it and/or modify it under the terms of the
   GNU General Public License as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   KeeperRL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
   even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along with this program.
   If not, see http://www.gnu.org/licenses/ . */

#include "stdafx.h"

#include "controller.h"
#include "map_memory.h"
#include "creature.h"

template <class Archive> 
void DoNothingController::serialize(Archive& ar, const unsigned int version) {
  ar & SUBCLASS(Controller)
     & BOOST_SERIALIZATION_NVP(creature);
}

SERIALIZABLE(DoNothingController);

template <class Archive> 
void Controller::serialize(Archive& ar, const unsigned int version) {
  ar & SVAR(creature);
  CHECK_SERIAL;
}

SERIALIZABLE(Controller);

Controller::Controller(Creature* c) : creature(c) {
}

Controller* Controller::getPossessedController(Creature* c) {
  return new DoNothingController(c);
}

ControllerFactory::ControllerFactory(function<Controller* (Creature*)> f) : fun(f) {}

PController ControllerFactory::get(Creature* c) {
  return PController(fun(c));
}

DoNothingController::DoNothingController(Creature* c) : Controller(c) {}

const MapMemory& DoNothingController::getMemory() const {
  return MapMemory::empty();
}

bool DoNothingController::isPlayer() const {
  return false;
}

void DoNothingController::you(MsgType type, const string& param) const {
}

void DoNothingController::you(const string& param) const {
}

void DoNothingController::makeMove() {
  creature->wait().perform();
}

void DoNothingController::onBump(Creature* c) {
}

