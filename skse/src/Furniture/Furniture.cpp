#include "Furniture.h"

#include "FurnitureTable.h"
#include "Graph/LookupTable.h"
#include "Util/ObjectRefUtil.h"
#include "Util/StringUtil.h"
#include "Util.h"

namespace Furniture {
    FurnitureType getFurnitureType(RE::TESObjectREFR* object, bool inUseCheck) {
        if (!object || object->IsDisabled() || object->IsMarkedForDeletion() || object->IsDeleted()) {
            return FurnitureType::NONE;
        }

        RE::TESBoundObject* base = object->GetBaseObject();
        if (!base) {
            return FurnitureType::NONE;
        }

        if (base->Is(RE::FormType::Furniture)) {
            if (inUseCheck && isFurnitureInUse(object, false)) {
                return FurnitureType::NONE;
            }

            if (object->HasKeyword(FurnitureTable::WICraftingSmithing)) {
                return FurnitureType::NONE;
            }

            if (object->HasKeyword(FurnitureTable::WICraftingAlchemy) || object->HasKeyword(FurnitureTable::WICraftingEnchanting) || object->HasKeyword(FurnitureTable::isLeanTable)) {
                return FurnitureType::TABLE;
            }

            if (object->HasKeyword(FurnitureTable::CraftingCookPot)) {
                return FurnitureType::COOKING_POT;
            }

            if (object->HasKeyword(FurnitureTable::IsTable) || object->HasKeyword(FurnitureTable::isWritingChair) || object->HasKeyword(FurnitureTable::FurnitureCounterLeanMarker)) {
                // chairs that are part of a table, or not a chair at all?!
                return FurnitureType::NONE;
            }

            if (base == FurnitureTable::WallLeanMarker) {
                return FurnitureType::WALL;
            }

            auto markers = getMarkers(object);

            if (markers.empty()) {
                return FurnitureType::NONE;
            }

            int chairMarkers = 0;

            for (auto& marker : markers) {
                if (marker.animationType.all(RE::BSFurnitureMarker::AnimationType::kSleep)) {
                    if (base == FurnitureTable::BYOHVampireCoffinVert01 || object->HasKeyword(FurnitureTable::isVampireCoffin) || object->HasKeyword(FurnitureTable::DLC1isVampireCoffinHorizontal) || object->HasKeyword(FurnitureTable::DLC1isVampireCoffinVertical)) {
                        // check for coffins
                        return FurnitureType::NONE;
                    }
                    return FurnitureType::BED;
                } else if (marker.animationType.all(RE::BSFurnitureMarker::AnimationType::kSit) && std::abs(marker.offset.z - 34) < 1) {
                    chairMarkers++;
                }
            }

            if (chairMarkers == 1) {
                return FurnitureType::CHAIR;
            } else if (chairMarkers > 1) {
                return FurnitureType::BENCH;
            }
        } else {
            if (FurnitureTable::OStimShelfList->HasForm(object->GetBaseObject()->formID)) {
                return FurnitureType::SHELF;
            }
            // TODO: check formlists
        }

        return FurnitureType::NONE;
    }

    std::vector<RE::TESObjectREFR*> findFurniture(int actorCount, RE::TESObjectREFR* centerRef, float radius, float sameFloor) {
        std::vector<RE::TESObjectREFR*> ret(7);
        if (!centerRef) {
            return ret;
        }

        auto centerPos = centerRef->GetPosition();

        util::iterate_attached_cells(centerPos, radius, [&](RE::TESObjectREFR& ref) {
            auto refPos = ref.GetPosition();

            if (sameFloor == 0.0 || std::fabs(centerPos.z - refPos.z) <= sameFloor) {
                FurnitureType type = getFurnitureType(&ref, true);
                if (type == FurnitureType::NONE || type != FurnitureType::BED && !Graph::LookupTable::hasNodes(type, actorCount)) {
                    return RE::BSContainer::ForEachResult::kContinue;
                }

                int index = type - 1;
                if (!ret[index] || centerPos.GetDistance(refPos) < centerPos.GetDistance(ret[index]->GetPosition())) {
                    ret[index] = &ref;
                }
            }

            return RE::BSContainer::ForEachResult::kContinue;
        });

        return ret;
    }

    std::vector<float> getOffset(RE::TESObjectREFR* object) {
        std::vector<float> ret = {0,0,0,0};

        if (!object) {
            return ret;
        }

        if (object->GetBaseObject()->Is(RE::FormType::Furniture)) {
            auto markers = getMarkers(object);

            if (markers.empty()) {
                return ret;
            }

            ret[0] = markers[0].offset.x;
            ret[1] = markers[0].offset.y;
            ret[2] = markers[0].offset.z;
            ret[3] = markers[0].heading;

            switch (getFurnitureType(object, false)) {
                case BED:
                    if (!object->HasKeyword(FurnitureTable::FurnitureBedRoll)) {
                        ret[1] += 50;
                        ret[2] += 8;
                    }
                    ret[0] = 0;
                    ret[3] += 2 * std::acos(0); // basically += math.pi
                    break;
                case BENCH:
                    ret[0] = 0;
                case CHAIR:
                    // all sit markers on benches and chairs have a z offset of ~34, but animations are centered on the ground
                    ret[2] = 0;
                    break;
                case TABLE:
                    if (object->HasKeyword(FurnitureTable::isLeanTable)) {
                        // temporary solution until I implement scaling on a per furniture basis
                        ret[2] -= 3.5;

                        // specific offsets for the BBLS railings
                        if (ObjectRefUtil::isInBBLS(object)) {
                            ret[1] -= 7.5;
                            ret[2] -= 3;
                        }
                    }
                    break;
                default:
                    break;
            }

            float refScale = object->GetReferenceRuntimeData().refScale / 100.0f;
            if (refScale != 1) {
                ret[0] *= refScale;
                ret[1] *= refScale;
                ret[2] *= refScale;
            }
        }

        return ret;
    }

    RE::BSTArray<RE::BSFurnitureMarker> Furniture::getMarkers(RE::TESObjectREFR* object) {
        auto root = object->Get3D();
        if (!root) {
            return {};
        }

        auto extra = root->GetExtraData("FRN");
        if (!extra) {
            return {};
        }

        auto node = netimmerse_cast<RE::BSFurnitureMarkerNode*>(extra);
        if (!node) {
            return {};
        }

        return node->markers;
    }

    bool Furniture::isFurnitureInUse(RE::TESObjectREFR* object, bool ignoreReserved) {
        return IsFurnitureInUse(nullptr, 0, object, ignoreReserved);
    }

    void Furniture::lockFurniture(RE::TESObjectREFR* furniture, RE::Actor* actor) {
        furniture->SetActivationBlocked(true);

        auto size = getMarkers(furniture).size();
        auto markers = furniture->extraList.GetByType<RE::ExtraUsedMarkers>();
        if (!markers) {
            return;
        }

        for (uint32_t i = 0; i < size; i++) {
            RE::MarkerUsedData data{.actorinMarker = RE::BSPointerHandleManagerInterface<RE::Actor>::GetHandle(actor), .markerID = i, .expiration = {.timeStamp=FLT_MAX}};
            markers->usedMarkers.push_back(data);
        }
    }

    void Furniture::freeFurniture(RE::TESObjectREFR* furniture) {
        furniture->SetActivationBlocked(false);

        auto markers = furniture->extraList.GetByType<RE::ExtraUsedMarkers>();
        if (!markers) {
            return;
        }
        markers->usedMarkers.clear();
    }

    void Furniture::resetClutter(RE::TESObjectREFR* centerRef, float radius) {
        if (auto TES = RE::TES::GetSingleton(); TES) {
            TES->ForEachReferenceInRange(centerRef, radius, [&](RE::TESObjectREFR& ref) {
                if (!ref.Is3DLoaded() || ref.IsDynamicForm() || ref.IsDisabled() || ref.IsMarkedForDeletion() || ref.IsDeleted() || ObjectRefUtil::getMotionType(&ref) == 4) {
                    return RE::BSContainer::ForEachResult::kContinue;
                }

                auto base = ref.GetBaseObject();
                if (!base) {
                    return RE::BSContainer::ForEachResult::kContinue;
                }

                for (auto type : FurnitureTable::clutterForms) {
                    if (ref.Is(type) || base->Is(type)) {
                        const auto skyrimVM = RE::SkyrimVM::GetSingleton();
                        auto vm = skyrimVM ? skyrimVM->impl : nullptr;
                        if (vm) {
                            RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> callback;
                            auto args = RE::MakeFunctionArguments();
                            auto handle = skyrimVM->handlePolicy.GetHandleForObject(static_cast<RE::VMTypeID>(ref.FORMTYPE), &ref);
                            vm->DispatchMethodCall2(handle, "ObjectReference", "MoveToMyEditorLocation", args, callback);
                        }
                        return RE::BSContainer::ForEachResult::kContinue;
                    }
                }
                
                return RE::BSContainer::ForEachResult::kContinue;
                });
        }
    }
}