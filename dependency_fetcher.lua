local DependencyFetcher = {}

function DependencyFetcher.FetchRepo(url, targetDir, commitID)
    if not os.isdir(targetDir) then
        print("[DependencyFetcher] Cloning " .. url .. " to " .. targetDir)
        
        -- Clone the repo using --depth 1 for efficient downloading
        local result = os.execute("git clone --depth 1 " .. url .. " " .. targetDir)
        if not result then
            error("[DependencyFetcher] Failed to clone repository: " .. url)
        end
        
        -- Since we often may need specific commits, we have an option to load a specific commit ID
        -- The way we can do this is since we've already cloned our repo, we jump to the working directory of our third party libraries
        -- and fetch the specific commit ID we want, then checkout the commit.
        if commitID then
            print("[DependencyFetcher] Checking out commit: " .. commitID)
            local checkoutCmd = string.format("git -C %s fetch --depth 1 origin %s && git -C %s checkout %s", 
                                               targetDir, commitID, targetDir, commitID)
            local checkoutResult = os.execute(checkoutCmd)
            if not checkoutResult then
                error("[DependencyFetcher] Failed to checkout commit: " .. commitID)
            end

        end
    else
        print("[DependencyFetcher] Found existing: " .. targetDir)
    end

end

function DependencyFetcher.Setup()

    print("\n[DependencyFetcher] Fetching dependencies...")
    
    DependencyFetcher.FetchRepo("https://github.com/glfw/glfw.git",
        "CritEngine/thirdparty/libs/glfw")
    
    DependencyFetcher.FetchRepo("https://github.com/g-truc/glm.git", 
        "CritEngine/thirdparty/libs/glm",
        "a40974fb8647717b15dbfd212b4a33845a405a88") 

    DependencyFetcher.FetchRepo("https://github.com/ocornut/imgui.git", 
        "CritEngine/thirdparty/libs/imgui")
    
    print("[DependencyFetcher] Dependency fetch completed!\n")
    
end

return DependencyFetcher