#pragma once

#include "Core/Core.h"
#include "Furniture/Furniture.h"
#include "Util/ActorUtil.h"
#include "Util/FormUtil.h"
#include "Util/ObjectRefUtil.h"
#include "Util/LookupTable.h"

namespace Serialization {
    enum DeserializationError {
        VEHICLE = 1 << 0,
        FURNITURE = 1 << 1,
        ACTOR = 1 << 2,
        EQUIPOBJECT = 1 << 3
    };

    struct OldThreadActor {
        inline static void deserialize(SKSE::SerializationInterface* serial, std::vector<OldThreadActor> &actors, uint32_t &errors) {
            OldThreadActor threadActor;

            RE::FormID oldID;
            RE::FormID newID;

            serial->ReadRecordData(&oldID, sizeof(oldID));
            if (serial->ResolveFormID(oldID, newID)) {
                RE::TESForm* form = RE::TESForm::LookupByID(newID);
                if (form) {
                    if (form->Is(RE::Actor::FORMTYPE)) {
                        threadActor.actor = form->As<RE::Actor>();
                        logger::info("found actor {}", threadActor.actor->GetDisplayFullName());
                        errors |= DeserializationError::ACTOR;
                    } else {
                        logger::warn("not an actor id: {}", newID);
                        errors |= DeserializationError::ACTOR;
                    }
                } else {
                    logger::warn("cannot find actor with id: {}", newID);
                    errors |= DeserializationError::ACTOR;
                }
            } else {
                logger::warn("cannot resolve actor id {:x}, missing mod?", oldID);
                errors |= DeserializationError::ACTOR;
            }

            size_t size;
            serial->ReadRecordData(&size, sizeof(size));
            for (int i = 0; i < size; i++) {
                serial->ReadRecordData(&oldID, sizeof(oldID));
                if (serial->ResolveFormID(oldID, newID)) {
                    RE::TESForm* form = RE::TESForm::LookupByID(newID);
                    if (form) {
                        if (form->Is(RE::TESObjectARMO::FORMTYPE)) {
                            threadActor.equipObjects.push_back(form->As<RE::TESObjectARMO>());
                        } else {
                            logger::warn("not an equip object id: {}", newID);
                            errors |= DeserializationError::EQUIPOBJECT;
                        }
                    } else {
                        logger::warn("cannot find equip object with id: {}", newID);
                        errors |= DeserializationError::EQUIPOBJECT;
                    }
                } else {
                    logger::warn("cannot resolve equip object id {:x}, missing mod?", oldID);
                    errors |= DeserializationError::EQUIPOBJECT;
                }
            }

            if (threadActor.actor) {
                actors.push_back(threadActor);
            }
        }

        RE::Actor* actor = nullptr;
        std::vector<RE::TESObjectARMO*> equipObjects;

        inline void serialize(SKSE::SerializationInterface* serial) {
            RE::FormID actorID = actor->GetFormID();
            serial->WriteRecordData(&actorID, sizeof(actorID));

            size_t size = equipObjects.size();
            serial->WriteRecordData(&size, sizeof(size));
            for (RE::TESObjectARMO* equipObject : equipObjects) {
                RE::FormID objectID = equipObject->GetFormID();
                serial->WriteRecordData(&objectID, sizeof(objectID));
            }
        }

        inline void free() {
            OStim::freeActor(actor, true);
            for (RE::TESObjectARMO* equipObject : equipObjects) {
                ActorUtil::unequipItem(actor, equipObject);
                ObjectRefUtil::removeItem(actor, equipObject);
            }
            if (!equipObjects.empty()) {
                ActorUtil::queueNiNodeUpdate(actor);
            }
        }
    };

    struct OldThread {
        inline static void deserialize(SKSE::SerializationInterface* serial, std::vector<OldThread> &threads, uint32_t &errors) {
            OldThread thread;

            RE::FormID oldID;
            RE::FormID newID;

            serial->ReadRecordData(&oldID, sizeof(oldID));
            if (serial->ResolveFormID(oldID, newID)) {
                RE::TESForm* form = RE::TESForm::LookupByID(newID);
                if (form) {
                    RE::TESObjectREFR* vehicle = form->As<RE::TESObjectREFR>();
                    if (vehicle) {
                        thread.vehicle = vehicle;
                    } else {
                        logger::error("not a vehicle id: {}", newID);
                        errors |= DeserializationError::VEHICLE;
                    }
                } else {
                    logger::error("cannot find vehicle with id: {}", newID);
                    errors |= DeserializationError::VEHICLE;
                }
            } else {
                logger::error("cannot resolve vehicle id {:x}", oldID);
                errors |= DeserializationError::VEHICLE;
            }

            serial->ReadRecordData(&oldID, sizeof(oldID));
            if (oldID != 0) {
                if (serial->ResolveFormID(oldID, newID)) {
                    RE::TESForm* form = RE::TESForm::LookupByID(newID);
                    if (form) {
                        RE::TESObjectREFR* furniture = form->As<RE::TESObjectREFR>();
                        if (furniture) {
                            thread.furniture = furniture;
                        } else {
                            logger::error("not a furniture id: {}", newID);
                            errors |= DeserializationError::FURNITURE;
                        }
                    } else {
                        logger::error("cannot find furniture with id: {}", newID);
                        errors |= DeserializationError::FURNITURE;
                    }
                } else {
                    logger::error("cannot resolve furniture id {:x}", oldID);
                    errors |= DeserializationError::FURNITURE;
                }
            }

            serial->ReadRecordData(&oldID, sizeof(oldID));
            if (oldID != 0) {
                if (serial->ResolveFormID(oldID, newID)) {
                    RE::TESForm* form = RE::TESForm::LookupByID(newID);
                    if (form) {
                        thread.furnitureOwner = form;
                    } else {
                        logger::error("cannot find furniture owner with id: {}", newID);
                        errors |= DeserializationError::FURNITURE;
                    }
                } else {
                    logger::error("cannot resolve furniture owner id {:x}", oldID);
                    errors |= DeserializationError::FURNITURE;
                }
            }
            
            size_t size;
            serial->ReadRecordData(&size, sizeof(size));
            for (int i = 0; i < size; i++) {
                OldThreadActor::deserialize(serial, thread.actors, errors);
            }

            threads.push_back(thread);
        }

        RE::TESObjectREFR* vehicle = nullptr;
        RE::TESObjectREFR* furniture = nullptr;
        RE::TESForm* furnitureOwner = nullptr;
        std::vector<OldThreadActor> actors;

        inline void serialize(SKSE::SerializationInterface* serial) {
            RE::FormID vehicleID = vehicle->GetFormID();
            serial->WriteRecordData(&vehicleID, sizeof(vehicleID));

            RE::FormID furnitureID = furniture ? furniture->GetFormID() : 0;
            serial->WriteRecordData(&furnitureID, sizeof(furnitureID));

            RE::FormID furnitureOwnerID = furnitureOwner ? furnitureOwner->GetFormID() : 0;
            serial->WriteRecordData(&furnitureOwnerID, sizeof(furnitureOwnerID));

            size_t size = actors.size();
            serial->WriteRecordData(&size, sizeof(size));
            for (OldThreadActor actor : actors) {
                actor.serialize(serial);
            }
        }

        inline void close() {
            if (vehicle) {
                vehicle->Disable();
                vehicle->SetDelete(true);
            }

            if (furniture) {
                Furniture::freeFurniture(furniture, furnitureOwner);   
            }

            bool playerThread = false;

            for (OldThreadActor actor : actors) {
                actor.free();
                playerThread |= actor.actor->IsPlayerRef();
            }

            if (playerThread) {
                FormUtil::sendModEvent(Util::LookupTable::OSexIntegrationMainQuest, "ostim_end", "", -0);
                FormUtil::sendModEvent(Util::LookupTable::OSexIntegrationMainQuest, "ostim_totalend", "", 0);
            }
        }
    };
}