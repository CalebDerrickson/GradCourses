Creating an Overleaf project from an existing Git repository
If you have an existing repository, you can add an Overleaf project as a remote repository. There are a few commands below that you'll need to run to get things started.

First, we assume you have git repo on your computer. (It might also be hosted on GitHub, GitLab, or Bitbucket, etc., but you need to have it cloned locally, too.)
Create a new project on Overleaf. Since any content in the project will be overwritten, use the 'Blank' Project option when creating this project.
Find the Git URL for the project (see above).


Add the Git URL for the project as a remote in your local project.
$ cd my-paper
$ git remote add overleaf <GIT-URL>
(Be sure to use your own link, not the example link.)

Pull the latest content from the Overleaf project and merge it into your master branch.
$ git checkout main
$ git pull overleaf master --allow-unrelated-histories
Revert the merge to get rid of the files in the existing Overleaf project.
Potentially not needed
$ git revert --mainline 1 HEAD
Push your project to Overleaf.
$ git push overleaf master
Visit the project on Overleaf. Your changes should be there. (You may have to open the Overleaf Project menu to find which file has been set as the main document file, and possibly change this initial selection.)
