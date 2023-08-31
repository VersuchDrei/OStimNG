#include "Furniture.h"

#include "FurnitureTable.h"
#include "Graph/GraphTable.h"
#include "MCM/MCMTable.h"
#include "Util/LookupTable.h"
#include "Util/ObjectRefUtil.h"
#include "Util/StringUtil.h"
#include "Util.h"

namespace Furniture {
   std::vector<std::pair<FurnitureType*, GameAPI::GameObject>> findFurniture(int actorCount, GameAPI::GameObject center, float radius, float sameFloor) {
        if (!center) {
            return {};
        }

        std::unordered_map<FurnitureType*, GameAPI::GameObject> furniture;

        auto centerPos = center.form->GetPosition();
        util::iterate_attached_cells(centerPos, radius, [&](RE::TESObjectREFR& ref) {
            GameAPI::GameObject object = &ref;
            auto refPos = ref.GetPosition();

            if (sameFloor == 0.0 || std::fabs(centerPos.z - ref.GetPosition().z) <= sameFloor) {
                FurnitureType* type = FurnitureTable::getFurnitureType(&ref, true)->getListType();
                if (type->id == "none" || !Graph::GraphTable::hasNodes(type->getMasterType(), actorCount)) {
                    return RE::BSContainer::ForEachResult::kContinue;
                }

                if (furniture.contains(type) && furniture[type].getSquaredDistance(center) < object.getSquaredDistance(center)) {
                    return RE::BSContainer::ForEachResult::kContinue;
                }

                furniture[type] = object;
            }

            return RE::BSContainer::ForEachResult::kContinue;
        });

        if (furniture.empty()) {
            return {};
        }

        std::vector<std::pair<FurnitureType*, GameAPI::GameObject>> ret;

        for (auto& [type, object] : furniture) {
            ret.push_back({type, object});
        }

        std::sort(ret.begin(), ret.end(), [&center](std::pair<Furniture::FurnitureType*, GameAPI::GameObject> a, std::pair<Furniture::FurnitureType*, GameAPI::GameObject> b) {
            return a.second.getSquaredDistance(center) < b.second.getSquaredDistance(center);
        });

        return ret;
    }

    GameAPI::GameObject findBed(RE::TESObjectREFR* centerRef, float radius, float sameFloor) {
        if (!centerRef) {
            return {};
        }

        FurnitureType* bedType = FurnitureTable::getFurnitureType("bed");
        if (!bedType) {
            return {};
        }

        GameAPI::GameObject bed = nullptr;

        auto centerPos = centerRef->GetPosition();

        util::iterate_attached_cells(centerPos, radius, [&](RE::TESObjectREFR& ref) {
            auto refPos = ref.GetPosition();

            if (sameFloor == 0.0 || std::fabs(centerPos.z - refPos.z) <= sameFloor) {
                FurnitureType* type = FurnitureTable::getFurnitureType(&ref, true);
                if (!type->isChildOf(bedType)) {
                    return RE::BSContainer::ForEachResult::kContinue;
                }

                // TODO GameObject
                if (!bed || centerPos.GetSquaredDistance(refPos) < centerPos.GetSquaredDistance(bed.form->GetPosition())) {
                    bed = &ref;
                }
            }

            return RE::BSContainer::ForEachResult::kContinue;
        });

        return bed;
    }

    FurnitureOffset getOffset(GameAPI::GameObject object) {
        if (!object) {
            return {};
        }

        GameAPI::GamePosition position = object.getMarkerOffset();
        FurnitureOffset offset = {position.x, position.y, position.z, position.r};

        FurnitureType* type = FurnitureTable::getFurnitureType(object, false);

        if (type->ignoreMarkerOffsetX) {
            offset.x = 0;
        }
        offset.x += type->offsetX;
        if (type->offsetXGlobal) {
            offset.x += type->offsetXGlobal.getValue();
        }

        if (type->ignoreMarkerOffsetY) {
            offset.y = 0;
        }
        offset.y += type->offsetY;
        if (type->offsetYGlobal) {
            offset.y += type->offsetYGlobal.getValue();
        }

        if (type->ignoreMarkerOffsetZ) {
            offset.z = 0;
        }
        offset.z += type->offsetZ;
        if (type->offsetZGlobal) {
            offset.z += type->offsetZGlobal.getValue();
        }

        offset.rotation += type->rotation;

        offset.scale = type->multiplyScale;

        float scale = object.getScale();
        if (scale != 1.0f) {
            offset.x *= scale;
            offset.y *= scale;
            offset.z *= scale;
        }

        return offset;
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