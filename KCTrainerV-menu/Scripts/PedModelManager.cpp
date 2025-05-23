#include "pch.h"
#include "PedModelManager.h"

#ifdef PLAYER_SKIN_CHANGER

PedModelManager::PedModelManager() : m_isLoaded(false) {
    // Constructor initializes members.
}

void PedModelManager::LoadPedsFromFile(const std::string& filePath) {
    if (m_isLoaded) {
        std::string warningString = "PedModelManager: Peds already loaded. Call Clear() first if reloading.";
        LOG(WARN, warningString);
        return;
    }

    tinyxml2::XMLDocument doc;
    // Load the XML file. LoadFile takes const char*.
    if (doc.LoadFile(filePath.c_str()) != tinyxml2::XML_SUCCESS) {
        // TODO Fix this
        //std::string errorString = std::format("PedModelManager: Failed to load XML file '{}'. Error: {}", filePath, doc);
        std::string errorString = std::format("PedModelManager: Failed to load XML file.");
        LOG(ERROR, errorString);
        return;
    }

    // Get the root element (e.g., <PedList>)
    tinyxml2::XMLElement* root = doc.RootElement();
    if (!root || std::string(root->Name()) != "PedList") {
        LOG(ERROR, "PedModelManager: Invalid XML root element. Expected 'PedList'.");
        return;
    }

    // Iterate through each <Category> element
    for (tinyxml2::XMLElement* categoryElement = root->FirstChildElement("Category");
        categoryElement != nullptr;
        categoryElement = categoryElement->NextSiblingElement("Category")) {

        const char* categoryNameAttr = categoryElement->Attribute("name");
        if (!categoryNameAttr) {
            LOG(WARN, "PedModelManager: Skipping Category with no 'name' attribute.");
            continue;
        }

        PedCategory currentCategory;
        currentCategory.name = categoryNameAttr;
        std::string loadCategoryString = std::format("PedModelManager: Loading category: {}", currentCategory.name);
        LOG(INFO, loadCategoryString);

        // Iterate through each <Ped> element within the current category
        for (tinyxml2::XMLElement* pedElement = categoryElement->FirstChildElement("Ped");
            pedElement != nullptr;
            pedElement = pedElement->NextSiblingElement("Ped")) {

            const char* pedNameAttr = pedElement->Attribute("name");
            const char* pedCaptionAttr = pedElement->Attribute("caption");

            if (!pedNameAttr || !pedCaptionAttr) {
                std::string warningString = std::format("PedModelManager: Skipping Ped with missing 'name' or 'caption' attribute in category '%s'.", currentCategory.name);
                LOG(WARN, warningString);
                continue;
            }

            PedInfo pedInfo;
            pedInfo.modelName = pedNameAttr;
            pedInfo.caption = pedCaptionAttr;
            pedInfo.hash = MISC::GET_HASH_KEY(pedInfo.modelName.c_str()); // Calculate hash at runtime

            currentCategory.peds.push_back(pedInfo);
        }
        m_categories.push_back(currentCategory); // Add populated category to manager's list
    }

    m_isLoaded = true;
    int categoriesSize = m_categories.size();
    int totalPeds = std::accumulate(m_categories.begin(), m_categories.end(), 0,
        [](size_t sum, const PedCategory& cat) { return sum + cat.peds.size(); });

    std::string successMessage = std::format("PedModelManager: Successfully loaded {} categories with a total of {} peds.",
        categoriesSize, totalPeds);
    LOG(INFO, successMessage);
}

const std::vector<PedCategory>& PedModelManager::GetAllCategories() const {
    return m_categories;
}

const PedCategory* PedModelManager::GetCategory(const std::string& categoryName) const {
    for (const auto& category : m_categories) {
        if (category.name == categoryName) {
            return &category;
        }
    }
    return nullptr; // Category not found
}

#endif