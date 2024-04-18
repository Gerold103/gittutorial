# Git Patch Series tutorial

## Introduction

The tutorial is performed as a test task which shows how to build patch series in form multiple independent clear atomic commits, and most interestingly - how to work with such a patchset: change commit order, add new commits, drop not needed ones, change commit messages, modify code in the middle of the series, and all sorts of other changes. All that while not requiring to redo the patchset from scratch.

Small atomic commits forming a story-like patch series are 1) actually quite easy to make after some training, 2) very easy to review step by step, 3) good for git history of your repo.

This tutorial is only about the linear patchset structure. It doesn't involve any work with merge-commits. The linear patchset is typically about a certain bugfix or a feature and doesn't contain any merge commits. These can appear in the main/master branch when the sub-branches are being merged into the main stream, but they are not supposed to appear in the sub-branches as a part of your own patchset which is in work yet.

## Commands

These are the commands which are needed for working on a patchset.

* **`git stash`** - all the current not-committed changes can be saved into a temporary storage without creating any commits, and then applied at any other moment. Essentially, it saves the `git diff` and discards the changes. Then `git stash apply/pop` applies the saved diff.

* **`git commit --amend`** - merge the current changes into the current head-commit instead of creating a new one. This is very useful when working on a next commit and doing it in small parts when all should in the end become one commit.

* **`git reset [<hash>]`** - soft reset to the given commit will remove all the commits on top of this one while keeping their changes. The changes won't disappear. Will be simply *"not staged for commit"* again. By default the hash is `HEAD`. Then it simply makes all *"to be committed"* changes *"not staged for commit"* again.

* **`git reset --hard [<hash>]`** - hard reset will delete all the commits on top of this one with all their changes. It is very useful when did some work and decided to discard it all at once, or want to get rid of some latest commits. By default it will delete all changes, both staged and just modified. Only untracked files remain intact.

* **`git rebase -i <hash>`** - this is the main command for building patchsets. It is used for re-applying all the commits of this branch on top of the given hash or branch name. Which makes it the tool for refreshing your branch on top of the latest main/master/develop, and for rebasing the branch on itself to change some commits in the middle of it.

* **`git blame <file>`** - for a given file it prints its content with commit hash and author name near each line showing when it was changed last time. It is very helpful to find why something was done, or when was a bug introduced. If the culprit is not the last commit changing this line, then can jump to the commit before this one, and make `blame` again to find the previous one, and so on. Sadly, in console it might be complicated to do all these jumps. Easiest way is in the repository's web-page, where these `blame->commit->prev_commit->blame->...` steps can be done very quick.

This is all not counting the very basic commands like plain `git commit`, `git add`, etc, which are still needed and useful, and are too simply to consider them all here individually.

## Project

This repository offers a task which can help one to use all these commands from above.

There are modules:
* `core` - the basic functionality used by everything.
* `bar` - a library offering `struct ObjectBar` and its stuff.
* `foo` - a library offering `struct ObjectFoo` and also stuff.
* `a.out` - the executable which uses all these libraries.

This application is already fully implemented in a clean patch series on the branch `master`.

However the original draft implementation of this all was a dirty branch `source` which implements the same, but with some problems:
- Some debug prints are not deleted.
- Certain commits are not atomic, and others are too highly granulated.
- Commit order is not good.
- Not all code is needed.

Lets compare these branches code-wise.
```Bash
git checkout source
git diff master
```
<details><summary>Diff output</summary>

```Diff
diff --git a/core.cpp b/core.cpp
index 2a5d3d1..117a4e3 100644
--- a/core.cpp
+++ b/core.cpp
@@ -1,5 +1,7 @@
 #include "core.h"
 
+#include <iostream>
+
 static std::string_view keys[] = {
 	"Kez",
 	"Kex",
@@ -35,15 +37,19 @@ FlibberCollection::flibber()
 std::string
 FlibberCollection::toString(uint8_t tabs) const
 {
+	std::cout << "----------------------------- toString1\n";
 	if (myFlibbers.size() == 0)
-		return {};
+		return "<empty>\n";
+
+	std::cout << "----------------------------- toString2\n";
 	std::string tab;
 	for (uint8_t i = 0; i < tabs; ++i)
 		tab += '\t';
 
-	std::string res = tab + "flibbers:\n";
-	for (const Flibber& fli : myFlibbers)
-		res += tab + '\t' + fli.myKey + ": " + fli.myValue + '\n';
+	std::cout << "----------------------------- toString3\n";
+	std::string res = tab + "flibbers: <PLACEHOLDER>\n";
+
+	std::cout << "----------------------------- toString4\n";
 	return res;
 }
 
diff --git a/main.cpp b/main.cpp
index d79e1e2..69ae939 100644
--- a/main.cpp
+++ b/main.cpp
@@ -8,7 +8,7 @@ main()
 {
 	std::cout << "#### Foo\n";
 
-	ObjectFoo f = makeFoo();
+	ObjectFoo f = makeFoo(10);
 	std::cout << f.toString(1) << '\n';
 
 	std::cout << "# Perform wibble-zorbling\n";
diff --git a/moduleBar.h b/moduleBar.h
index 06b52b4..a28b63e 100644
--- a/moduleBar.h
+++ b/moduleBar.h
@@ -5,7 +5,7 @@
 struct ObjectBar : public FlibberCollection
 {
 	void splinx();
-	void yibble() { myYibble = (myYibble + 1) * 2; }
+	void yibble() { myYibble = myYibble + 1; }
 	std::string toString(uint8_t tabs) const;
 
 	std::string mySplinx = "Emptio";
diff --git a/moduleFoo.cpp b/moduleFoo.cpp
index c94eb4c..bd2e17e 100644
--- a/moduleFoo.cpp
+++ b/moduleFoo.cpp
@@ -21,7 +21,9 @@ ObjectFoo::toString(uint8_t tabs) const
 }
 
 ObjectFoo
-makeFoo()
+makeFoo(int wibble)
 {
-	return ObjectFoo();
+	ObjectFoo f;
+	f.myWibble = wibble;
+	return f;
 }
diff --git a/moduleFoo.h b/moduleFoo.h
index 35a971c..cdafafe 100644
--- a/moduleFoo.h
+++ b/moduleFoo.h
@@ -15,4 +15,4 @@ struct ObjectFoo : public FlibberCollection
 	char myZorble = 'a';
 };
 
-ObjectFoo makeFoo();
+ObjectFoo makeFoo(int wibble);
```
</details>

These are easy to fix, but we need to do it clean. The changes should go into the commits where they belong. Lets see the git-log difference.
```Bash
git log --oneline source
git log --oneline master
```
```Bash
############################# Source #############################
hash "[wip] New splinxes for Bar"
hash "[wip] FlibberCollection debug"
hash "[wip] toString() for Foo and Bar"
hash "Introduce FlibberCollection"
hash "[wip] Unwibbling?"
hash "[wip] Flibber in foo"
hash "[wip] Flibber in bar"
hash "[wip] Flibber test"
hash "[wip] Introduce core library with flibber"
hash "Introduce bar module"
hash "[wip] Specify wibble in makeFoo()?"
hash "Initial commit"

############################# Master #############################
hash "FlibberCollection::toString()"
hash "Make Foo responsible for to-string"
hash "Make Bar responsible for to-string"
hash "Introduce FlibberCollection"
hash "Support unwibbling of Foo"
hash "Add flibbering support"
hash "Introduce bar module"
hash "Initial commit"
```

The source git log is quite dirty. It has to be cleaned and the code changes must be made in the appropriate commits. Otherwise review of this `source` branch would be a nightmare. It simply won't be possible commit-by-commit, because the commits make little sense.

## Tasks

Lets summarize what has to be done.

1. The commit **`"[wip] New splinxes for Bar"`** must be merged into **`"Introduce bar module"`**. Apparently, in the latter we forgot these splinxes, added them later, but they really belong to the bar intro commit.
2. The commit **`"[wip] FlibberCollection debug"`** must be changed. It contains debug prints, a mistake, and its message is crap.
3. The commit **`"[wip] Unwibbling?"`** needs a new message. The commit itself is fine, but the message is bad.
4. The commits **`"[wip] Flibber in foo"`**, **`"[wip] Flibber in bar"`**, **`"[wip] Flibber test"`**, **`"[wip] Introduce core library with flibber"`** must be merged (squashed). They are too granular. And tests are usually supposed to be in the same commit as code changes.
5. The commit **`"[wip] Specify wibble in makeFoo()?"`** must be deleted. The `makeFoo()` argument isn't needed.
6. The commit **`"[wip] toString() for Foo and Bar"`** must be split into 2: for foo and bar separately.
7. The commit **`"Introduce bar module"`** must change `ObjectBar::yibble()` from `myYibble = myYibble + 1;` to `myYibble = (myYibble + 1) * 2;`.
8. In the end lets inspect with `git-blame` the results.

Lets do them. You can also do the steps yourself when you are confident enough, to train more.

Before start, lets checkout to a new branch where we wouldn't ruin the original `source` and would be able to get back to start if stuff is too broken.
```Bash
git checkout -b new-source
```

---
### Step 1, commit move down and fixup
#### *The commit **`"[wip] New splinxes for Bar"`** must be merged into **`"Introduce bar module"`**.*

Lets see the log: `git log --oneline`.
```Bash
hash     "[wip] New splinxes for Bar" # ->--->--->--->---> Need to merge it here.
hash     "[wip] FlibberCollection debug" #                           |
hash     "[wip] toString() for Foo and Bar" #                        |
hash     "Introduce FlibberCollection" #                             V
hash     "[wip] Unwibbling?" #                                       |
hash     "[wip] Flibber in foo" #                                    |
hash     "[wip] Flibber in bar" #                                    V
hash     "[wip] Flibber test" #                                      |
hash     "[wip] Introduce core library with flibber" #               |
#                                                                    V
hash-A   "Introduce bar module" # <---<---<---<---<---<---<---<---<--+
hash-B   "[wip] Specify wibble in makeFoo()?"

hash     "Initial commit"
```
Need to merge **`"[wip] New splinxes for Bar"`** into **`"Introduce bar module"`**. When doing any changes to older commits, you always need `git rebase -i`. The argument `-i` makes it interactive where before rebase you can do some tweaks.

Note, that when doing `git rebase -i <hash>`, the commits older and including `<hash>` will stay as is, and the newer ones are re-applied as you set them up to. Which means that if the oldest commit to change is `<hash>` itself, then you need to rebase to `<hash>~1` - previous one. Lets do that.
```Bash
git rebase -i <hash-A>~1 # Or same as `git rebase -i <hash-B>`.
```

This will put your `HEAD` to this state:
```Bash
# <HEAD> <--- YOU ARE HERE

hash "[wip] Specify wibble in makeFoo()?"
hash "Initial commit"
```
And git will open a text editor of your choice (normally `nano` or `vim`) with the following content:
```Bash
pick hash "Introduce bar module" # <---<---<---<---<---<+
pick hash "[wip] Introduce core library with flibber" # |
pick hash "[wip] Flibber test" #                        |
pick hash "[wip] Flibber in bar" #                      |
pick hash "[wip] Flibber in foo" #                      | Need to merge it.
pick hash "[wip] Unwibbling?" #                         |
pick hash "Introduce FlibberCollection" #               |
pick hash "[wip] toString() for Foo and Bar" #          |
pick hash "[wip] FlibberCollection debug" #             |
pick hash "[wip] New splinxes for Bar" # --->--->--->--->

# ... some tutorial commands here, a cheat-sheet with what can be changed and how.
```
This is a command-list which will get executed after you save and close the editor, from top to bottom, on top of the commit which you specified in `git rebase` command. Which is now our `HEAD`.

In order to merge **`"[wip] New splinxes for Bar"`** into **`"Introduce bar module"`** lets move them to each other in the command list and replace `pick` with `fixup` or `f`. The command `fixup` instead of applying this commit as is will merge it into the previous (above in the list) commit.
```Bash
pick hash "Introduce bar module"
# ^^^ fixup merges its commit into the previous one.
f    hash "[wip] New splinxes for Bar" # <---<---<--- This was moved.

pick hash "[wip] Introduce core library with flibber"
pick hash "[wip] Flibber test"
pick hash "[wip] Flibber in bar"
pick hash "[wip] Flibber in foo"
pick hash "[wip] Unwibbling?"
pick hash "Introduce FlibberCollection"
pick hash "[wip] toString() for Foo and Bar"
pick hash "[wip] FlibberCollection debug"
```
Now save and exit the editor, and all these commands are applied.

```Bash
# Successfully rebased and updated refs/heads/new-source.

$> git log --oneline
hash "[wip] FlibberCollection debug"
hash "[wip] toString() for Foo and Bar"
hash "Introduce FlibberCollection"
hash "[wip] Unwibbling?"
hash "[wip] Flibber in foo"
hash "[wip] Flibber in bar"
hash "[wip] Flibber test"
hash "[wip] Introduce core library with flibber"
hash "Introduce bar module"
hash "[wip] Specify wibble in makeFoo()?"
hash "Initial commit"
```

The commit **`"[wip] New splinxes for Bar"`** was merged into **`"Introduce bar module"`**. You can check that by looking at `git show` of this commit.

---
### Step 2, change top commit
#### *The commit **`"[wip] FlibberCollection debug"`** must be changed. It contains debug prints, a mistake, and its message is crap.*

Lets see what is wrong with it.
```Bash
git show # `HEAD` is default, and is the commit that we need to change.
```

<details><summary>Commit Data</summary>

```Diff
commit fdd3c226c97efc2ab7456daa06ab7333418a537b (HEAD -> new-source)
Author: Vladislav Shpilevoy <v.shpilevoy@tarantool.org>
Date:   Thu Apr 18 13:03:17 2024 +0200

    [wip] FlibberCollection debug

diff --git a/core.cpp b/core.cpp
index f30ffbd..117a4e3 100644
--- a/core.cpp
+++ b/core.cpp
@@ -1,5 +1,7 @@
 #include "core.h"
 
+#include <iostream>
+
 static std::string_view keys[] = {
        "Kez",
        "Kex",
@@ -32,6 +34,25 @@ FlibberCollection::flibber()
        myFlibbers.push_back(makeFlibber());
 }
 
+std::string
+FlibberCollection::toString(uint8_t tabs) const
+{
+       std::cout << "----------------------------- toString1\n";
+       if (myFlibbers.size() == 0)
+               return "<empty>\n";
+
+       std::cout << "----------------------------- toString2\n";
+       std::string tab;
+       for (uint8_t i = 0; i < tabs; ++i)
+               tab += '\t';
+
+       std::cout << "----------------------------- toString3\n";
+       std::string res = tab + "flibbers: <PLACEHOLDER>\n";
+
+       std::cout << "----------------------------- toString4\n";
+       return res;
+}
...
...
```
</details>

It contains debug prints, missing functionality of printing the flibbers, and its message is bad. Lets fix the code first. It has to be done manually. And this is the diff:
<details><summary>Diff</summary>

```Diff
diff --git a/core.cpp b/core.cpp
index 117a4e3..2a5d3d1 100644
--- a/core.cpp
+++ b/core.cpp
@@ -1,7 +1,5 @@
 #include "core.h"
 
-#include <iostream>
-
 static std::string_view keys[] = {
 	"Kez",
 	"Kex",
@@ -37,19 +35,15 @@ FlibberCollection::flibber()
 std::string
 FlibberCollection::toString(uint8_t tabs) const
 {
-	std::cout << "----------------------------- toString1\n";
 	if (myFlibbers.size() == 0)
-		return "<empty>\n";
-
-	std::cout << "----------------------------- toString2\n";
+		return {};
 	std::string tab;
 	for (uint8_t i = 0; i < tabs; ++i)
 		tab += '\t';
 
-	std::cout << "----------------------------- toString3\n";
-	std::string res = tab + "flibbers: <PLACEHOLDER>\n";
-
-	std::cout << "----------------------------- toString4\n";
+	std::string res = tab + "flibbers:\n";
+	for (const Flibber& fli : myFlibbers)
+		res += tab + '\t' + fli.myKey + ": " + fli.myValue + '\n';
 	return res;
 }
```
</details>

Lets merge it into the top commit:
```Bash
git add -u         # Stage all tracked changes.
git commit --amend # Merge them into the `HEAD` commit.
```

If you make `git show` now, you will see the commit is changed. Now need to update the message. This is also the job for amend - `git commit --amend`. And use the commit message same as in `master`:
```
FlibberCollection::toString()

Prints the entire collection, good for code reuse.
```

Using `git show` one can confirm that the message and commit diff were updated and are now just like in `master` branch.

---
### Step 3, change old commit's message
#### *The commit **`"[wip] Unwibbling?"`** needs a new message. The commit itself is fine, but the message is bad.*

Lets see the log - `git log --oneline`.
```Bash
hash   "FlibberCollection::toString()"
hash   "[wip] toString() for Foo and Bar"
hash   "Introduce FlibberCollection"
hash   "[wip] Unwibbling?" # <---<---<---<---<---<--- Commit with bad message.
hash-A "[wip] Flibber in foo"
hash   "[wip] Flibber in bar"
hash   "[wip] Flibber test"
hash   "[wip] Introduce core library with flibber"
hash   "Introduce bar module"
hash   "[wip] Specify wibble in makeFoo()?"
hash   "Initial commit"
```

Need to update message of **`"[wip] Unwibbling?"`**. This is the job for interactive rebase.
```Bash
git rebase -i <hash-A>
```

The branch state now:
```Bash
# <HEAD> <--- YOU ARE HERE

hash "[wip] Flibber in foo"
hash "[wip] Flibber in bar"
hash "[wip] Flibber test"
hash "[wip] Introduce core library with flibber"
hash "Introduce bar module"
hash "[wip] Specify wibble in makeFoo()?"
hash "Initial commit"
```

The prepared commands are:
```Bash
pick hash "[wip] Unwibbling?" # <---<---<---<---<--- Need to change.

pick hash "Introduce FlibberCollection"
pick hash "[wip] toString() for Foo and Bar"
pick hash "FlibberCollection::toString()"
```

To change just the message and nothing else there is a special command `reword` or `r`. Lets use it:
```Bash
r    hash "[wip] Unwibbling?" # <---<---<---<---<--- Need to change.

pick hash "Introduce FlibberCollection"
pick hash "[wip] toString() for Foo and Bar"
pick hash "FlibberCollection::toString()"
```

Now after saving and exiting the editor the git applier will stop at this commit, open the editor again, and suggest to change the commit message. Lets use the same one as in `master`:
```
Support unwibbling of Foo

Sometimes wibble is too big, need to be able to unwibble it.
```

After saving and exiting the editor, the other commands are execute silently, and we are on top of the branch again. Using `git log` one can confirm that the message of the target commit was updated.

---
### Step 4, squash multiple commits
#### *The commits **`"[wip] Flibber in foo"`**, **`"[wip] Flibber in bar"`**, **`"[wip] Flibber test"`**, **`"[wip] Introduce core library with flibber"`** must be merged (squashed). They are too granular. And tests are usually supposed to be in the same commit as code changes.*

There are 4 commits which need to be squashed into one. Lets see the current state of the branch: `git log --oneline`.
```Bash
hash   "FlibberCollection::toString()"
hash   "[wip] toString() for Foo and Bar"
hash   "Introduce FlibberCollection"
hash   "Support unwibbling of Foo"
hash-A "[wip] Flibber in foo"
hash-B "[wip] Flibber in bar"
hash-C "[wip] Flibber test"
hash-D "[wip] Introduce core library with flibber"
hash-E "Introduce bar module"
hash   "[wip] Specify wibble in makeFoo()?"
hash   "Initial commit"
```
Need to squash `hash-A` - `hash-D`. This is the job for interactive rebase.
```Bash
git rebase -i <hash-E>
```

The branch state now:
```Bash
# <HEAD> <--- YOU ARE HERE

hash-E "Introduce bar module"
hash   "[wip] Specify wibble in makeFoo()?"
hash   "Initial commit"
```

The prepared commands are:
```Bash
# --------------------------------------------------------+
pick hash "[wip] Introduce core library with flibber" #   |
pick hash "[wip] Flibber test" #                          | To squash.
pick hash "[wip] Flibber in bar" #                        |
pick hash "[wip] Flibber in foo" #                        |
# --------------------------------------------------------+

pick hash "Support unwibbling of Foo"
pick hash "Introduce FlibberCollection"
pick hash "[wip] toString() for Foo and Bar"
pick hash "FlibberCollection::toString()"
```

Squash is the same as fixup, but it allows to change the commit message, which is right now necessary. `squash` or `s` command will merge the given commit into the previous one and open the editor to allow to type a new commit message. The commands have to be changed to this:

```Bash
# --------------------------------------------------------+
pick hash "[wip] Introduce core library with flibber" #   |
s    hash "[wip] Flibber test" #                          | To squash.
s    hash "[wip] Flibber in bar" #                        |
s    hash "[wip] Flibber in foo" #                        |
# --------------------------------------------------------+

pick hash "Support unwibbling of Foo"
pick hash "Introduce FlibberCollection"
pick hash "[wip] toString() for Foo and Bar"
pick hash "FlibberCollection::toString()"
```

After saving and exiting the editor the git applier will merge these commits and open the editor again where a new message can be typed. Lets type the same one as in `master` branch:
```
Add flibbering support

Now bar and foo are able to flibbe and maintain a queue of flibbers.
```

After saving and exiting the editor, the other commands are execute silently, and we are on top of the branch again. Using `git log` one can confirm that the commits were squashed into one and the message is updated. To see the new commit's content use `git show` on its hash.

---
### Step 5, delete a commit
#### *The commit **`"[wip] Specify wibble in makeFoo()?"`** must be deleted. The `makeFoo()` argument isn't needed.*

The mentioned commit isn't needed. Lets find it in `git log --oneline`:
```Bash
hash   "FlibberCollection::toString()"
hash   "[wip] toString() for Foo and Bar"
hash   "Introduce FlibberCollection"
hash   "Support unwibbling of Foo"
hash   "Add flibbering support"
hash   "Introduce bar module"
hash   "[wip] Specify wibble in makeFoo()?" # <---<---<--- To delete.
hash-A "Initial commit"
```

It is an old commit. Its deletion most likely will cause conflicts in newer commits. That would be a chance to try dealing with them. Lets rebase.
```Bash
git rebase -i <hash-A>
```

The branch state now:
```Bash
# <HEAD> <--- YOU ARE HERE

hash-A "Initial commit"
```

The prepared commands are:
```Bash
pick hash "[wip] Specify wibble in makeFoo()?" # <---<---<---<---<--- To delete.

pick hash "Introduce bar module"
pick hash "Add flibbering support"
pick hash "Support unwibbling of Foo"
pick hash "Introduce FlibberCollection"
pick hash "[wip] toString() for Foo and Bar"
pick hash "FlibberCollection::toString()"
```

To delete a commit it is enough to just drop its line from the command list:
```Bash
# Comment it out or delete - "[wip] Specify wibble in makeFoo()".

pick hash "Introduce bar module"
pick hash "Add flibbering support"
pick hash "Support unwibbling of Foo"
pick hash "Introduce FlibberCollection"
pick hash "[wip] toString() for Foo and Bar"
pick hash "FlibberCollection::toString()"
```

Lets save and exit the editor. Git applier will execute the remaining commands.

Unfortunately, not all commands could be applied. Git applier is stuck here:
```
Auto-merging main.cpp
CONFLICT (content): Merge conflict in main.cpp
Auto-merging moduleFoo.cpp
Auto-merging moduleFoo.h
error: could not apply <hash> [wip] toString() for Foo and Bar
hint: Resolve all conflicts manually, mark them as resolved with
hint: "git add/rm <conflicted_files>", then run "git rebase --continue".
hint: You can instead skip this commit: run "git rebase --skip".
hint: To abort and get back to the state before "git rebase", run "git rebase --abort".
Could not apply <hash> [wip] toString() for Foo and Bar
```

Lets see the state: `git status`.
```
interactive rebase in progress; onto <hash>
Last commands done (5 commands done):
   pick <hash> Introduce FlibberCollection
   pick <hash> [wip] toString() for Foo and Bar
  (see more in file .git/rebase-merge/done)
Next command to do (1 remaining command):
   pick <hash> FlibberCollection::toString()
  (use "git rebase --edit-todo" to view and edit)
You are currently rebasing branch 'new-source' on '<hash>'.
  (fix conflicts and then run "git rebase --continue")
  (use "git rebase --skip" to skip this patch)
  (use "git rebase --abort" to check out the original branch)

Changes to be committed:
  (use "git restore --staged <file>..." to unstage)
	modified:   ../moduleBar.cpp
	modified:   ../moduleBar.h
	modified:   ../moduleFoo.cpp
	modified:   ../moduleFoo.h

Unmerged paths:
  (use "git restore --staged <file>..." to unstage)
  (use "git add <file>..." to mark resolution)
	both modified:   ../main.cpp
```
It couldn't apply **"[wip] toString() for Foo and Bar"**. the conflict is in `both modified:   ../main.cpp`.

The current position in the branch: `git log --oneline`.
```Bash
# <HEAD> <--- YOU ARE HERE

hash "Introduce FlibberCollection"
hash "Support unwibbling of Foo"
hash "Add flibbering support"
hash "Introduce bar module"
hash "Initial commit"
```

And the remaining commands are:
```Bash
pick hash "[wip] toString() for Foo and Bar" # <---<---<--- Current one, broken.

pick hash "FlibberCollection::toString()" # <---<---<---<-- Next one.
```

The conflicts must be fixed. Lets have a look. Open the file `main.cpp` and see this:
```
int
main()
{
	std::cout << "#### Foo\n";

<<<<<<< HEAD
	ObjectFoo f = makeFoo();
	std::cout << "\tFoo's wibble: " << f.myWibble << '\n';
	std::cout << "\tFoo's zorble: " << f.myZorble << '\n';
	std::cout << '\n';
=======
	ObjectFoo f = makeFoo(10);
	std::cout << f.toString(1) << '\n';
>>>>>>> 285b6d6 ([wip] toString() for Foo and Bar)
```

The stuff between `<<<<<<< ... =======` is what is in `HEAD`, before the failed command.
The stuff between `======= ... >>>>>>>` is what the failed command wanted to do, but couldn't.
Most of the time you delete one of those versions fully and keep the other. Sometimes can join them into something new.
This time need to keep the new version, but drop the `makeFoo()` argument:

```C
int
main()
{
	std::cout << "#### Foo\n";

	ObjectFoo f = makeFoo();
	std::cout << f.toString(1) << '\n';
```

Now save `main.cpp`, check that the build works, and `a.out` runs fine. And finish the failed `pick` command.
```Bash
git add -u            # Stage all modified files, including conflicted ones.

git rebase --continue # Continue the rebase. The failed command is finished with
                      # the updated changes. And next commands are being executed.
```

See in `git log` that the commit is gone. Also check that its code is gone as well.

---
### Step 6, split commits and add new commits in the middle of patchset, stashing
#### *The commit **`"[wip] toString() for Foo and Bar"`** must be split into 2: for foo and bar separately.*

It contains changes which are completely independent, even though they are similar. Lets split them into separate commits. See the log `git log --oneline`:
```Bash
hash   "FlibberCollection::toString()"
hash-A "[wip] toString() for Foo and Bar" # <---<---<---< To split.
hash-B "Introduce FlibberCollection"
hash   "Support unwibbling of Foo"
hash   "Add flibbering support"
hash   "Introduce bar module"
hash   "Initial commit"
```

This is the job for rebase.
```Bash
git rebase -i <hash-B>
```

The branch state now:
```Bash
# <HEAD> <--- YOU ARE HERE

hash-B "Introduce FlibberCollection"
hash   "Support unwibbling of Foo"
hash   "Add flibbering support"
hash   "Introduce bar module"
hash   "Initial commit"
```

The prepared commands are:
```Bash
pick b5d3551 "[wip] toString() for Foo and Bar" # <---<---<--- To split.

pick a17e969 "FlibberCollection::toString()"
```

Splitting means commit editing. Lets change `pick` to `edit` or `e`.
```Bash
e    b5d3551 "[wip] toString() for Foo and Bar" # <---<---<--- To split.

pick a17e969 "FlibberCollection::toString()"
```

After saving and exiting the editor, the git applier stops at the `edit` command. Now this is the state:
```Bash
# <HEAD> <--- YOU ARE HERE

hash-A "[wip] toString() for Foo and Bar" # <---<---<---< To split.
hash-B "Introduce FlibberCollection"
hash   "Support unwibbling of Foo"
hash   "Add flibbering support"
hash   "Introduce bar module"
hash   "Initial commit"
```

Editing can include anything. Can amend new changes, change commit, title, etc. In the current case this commit has to be split in 2. Firstly, lets drop it while keeping its changes.
```Bash
git reset HEAD~1
```

This is the current state:
```
Changes not staged for commit:
  (use "git add <file>..." to update what will be committed)
  (use "git restore <file>..." to discard changes in working directory)
	modified:   ../main.cpp
	modified:   ../moduleBar.cpp
	modified:   ../moduleBar.h
	modified:   ../moduleFoo.cpp
	modified:   ../moduleFoo.h
```
```Bash
# <HEAD> <--- YOU ARE HERE

hash-B "Introduce FlibberCollection"
hash   "Support unwibbling of Foo"
hash   "Add flibbering support"
hash   "Introduce bar module"
hash   "Initial commit"
```

The changes of the commit to split are preserved as "not staged". The commit is gone now. Lets commit these changes in 2 separate commits. One for `foo` and another for `bar`. It would be easy to just `git add` the `foo` files, then commit, then add `bar` files, then also commit. But `main.cpp` has changes needed for both commits. How exactly to split it 2 is out of scope of this project. But the easiest way is to use `git add --patch`. It allows to interactively select individual git-diff hunks to stage for commit, and even allows their editing before staging.

This staging is done behind the scenes using `git add --patch`. `Bar` changes are added, lets see the state `git status`:
```
Changes to be committed:
  (use "git restore --staged <file>..." to unstage)
	modified:   ../main.cpp
	modified:   ../moduleBar.cpp
	modified:   ../moduleBar.h

Changes not staged for commit:
  (use "git add <file>..." to update what will be committed)
  (use "git restore <file>..." to discard changes in working directory)
	modified:   ../main.cpp
	modified:   ../moduleFoo.cpp
	modified:   ../moduleFoo.h
```

Now can commit the `bar` stuff like in `master`, with the given message, using `git commit`:
```
Make Bar responsible for to-string

Makes it easier to reuse the serialization.
````

Lets see the new state, `git status` + `git log --oneline`:
```
Changes not staged for commit:
  (use "git add <file>..." to update what will be committed)
  (use "git restore <file>..." to discard changes in working directory)
	modified:   ../main.cpp
	modified:   ../moduleFoo.cpp
	modified:   ../moduleFoo.h
```
```Bash
# <HEAD> <--- YOU ARE HERE

hash "Make Bar responsible for to-string" # <---<---<---<--- New commit.

hash "Introduce FlibberCollection"
hash "Support unwibbling of Foo"
hash "Add flibbering support"
hash "Introduce bar module"
hash "Initial commit"
```

Lets check if the new commit isn't broken without these unstaged changes.
* Stash the changes using `git stash`.
* Try to build and run the code.
* If any bugs, then fix them and amend into the current head.
* When done, unstash the `foo` changes with `git stash pop`.

Now can commit the foo changes with the message from `master`:
```
Make Foo responsible for to-string

Makes it easier to reuse the serialization.
```

The current state, `git log --oneline`:
```Bash
# <HEAD> <--- YOU ARE HERE

hash "Make Foo responsible for to-string" # <---<---<---<--- New commit.
hash "Make Bar responsible for to-string" # <---<---<---<--- New commit.

hash "Introduce FlibberCollection"
hash "Support unwibbling of Foo"
hash "Add flibbering support"
hash "Introduce bar module"
hash "Initial commit"
```

The new commits are here. Now lets continue the rebase to apply the next commits, `git rebase --continue`. All done now.

See in `git log` that the new commits took place of the old one.

---
### Step 7, change an old commit
#### *The commit **`"Introduce bar module"`** must change `ObjectBar::yibble()` from `myYibble = myYibble + 1;` to `myYibble = (myYibble + 1) * 2;`.*

Lets find the target commit in `git log --oneline`:
```Bash
hash   "FlibberCollection::toString()"
hash   "Make Foo responsible for to-string"
hash   "Make Bar responsible for to-string"
hash   "Introduce FlibberCollection"
hash   "Support unwibbling of Foo"
hash   "Add flibbering support"
hash-A "Introduce bar module" # <---<---<---<--- To edit.
hash-B "Initial commit"
```

In order to change this commit we need to rebase from below it:
```Bash
git rebase -i <hash-B>
```

This is the current state:
```Bash
# <HEAD> <--- YOU ARE HERE

hash-B "Initial commit"
```
and these commands are going to be applied:
```Bash
pick hash "Introduce bar module" # <---<---<---<--- To change.

pick hash "Add flibbering support"
pick hash "Support unwibbling of Foo"
pick hash "Introduce FlibberCollection"
pick hash "Make Bar responsible for to-string"
pick hash "Make Foo responsible for to-string"
pick hash "FlibberCollection::toString()"
```

Lets change `pick` to `e` in the first line to edit the first commit:
```Bash
e hash "Introduce bar module" # <---<---<---<--- To change.
```

After saving and exiting the editor, this is the current state:
```Bash
# <HEAD> <--- YOU ARE HERE

hash-A "Introduce bar module"
hash-B "Initial commit"
```

Lets change the needed place manually:
```Diff
diff --git a/moduleBar.h b/moduleBar.h
index c4afca5..2b7e6c1 100644
--- a/moduleBar.h
+++ b/moduleBar.h
@@ -5,7 +5,7 @@
 struct ObjectBar
 {
 	void splinx();
-	void yibble() { myYibble = myYibble + 1; }
+	void yibble() { myYibble = (myYibble + 1) * 2; }
 
 	std::string mySplinx = "Emptio";
 	uint64_t myYibble = 0;
```

And commit it into `HEAD` using `git commit -a --amend --no-edit`. The last option is to keep the old commit message as is.

Now lets continue with applying the next commands, `git rebase --continue`.

Unfortunately, one the next commits couldn't be `pick`ed so easily, there is a conflict:
```
Auto-merging moduleBar.h
CONFLICT (content): Merge conflict in moduleBar.h
error: could not apply <hash> Add flibbering support
hint: Resolve all conflicts manually, mark them as resolved with
hint: "git add/rm <conflicted_files>", then run "git rebase --continue".
hint: You can instead skip this commit: run "git rebase --skip".
hint: To abort and get back to the state before "git rebase", run "git rebase --abort".
Could not apply <hash> Add flibbering support
```

It was the next command, so `git log --oneline` looks like before. The next commit failed to apply and needs to be fixed. Lets see the conflict:
```
struct ObjectBar
{
	void splinx();
<<<<<<< HEAD
	void yibble() { myYibble = (myYibble + 1) * 2; }
=======
	void yibble() { myYibble = myYibble + 1; }
	void flibber();
>>>>>>> 7369c88 (Add flibbering support)
```

We should keep the new `yibble()` + the `flibber()` from the failed commit. Lets edit it to this:
```C++
struct ObjectBar
{
	void splinx();
	void yibble() { myYibble = (myYibble + 1) * 2; }
	void flibber();
```

Now compile and run the project to ensure that it works, then stage the new code and continue the rebase:
```Bash
git add -u
git rebase --continue
```

Another conflict, lets see:
```
Auto-merging moduleBar.h
CONFLICT (content): Merge conflict in moduleBar.h
error: could not apply <hash> Introduce FlibberCollection
hint: Resolve all conflicts manually, mark them as resolved with
hint: "git add/rm <conflicted_files>", then run "git rebase --continue".
hint: You can instead skip this commit: run "git rebase --skip".
hint: To abort and get back to the state before "git rebase", run "git rebase --abort".
Could not apply <hash> Introduce FlibberCollection
```

The state is:
```
Changes to be committed:
  (use "git restore --staged <file>..." to unstage)
	modified:   ../core.cpp
	modified:   ../core.h
	modified:   ../moduleBar.cpp
	modified:   ../moduleFoo.cpp
	modified:   ../moduleFoo.h

Unmerged paths:
  (use "git restore --staged <file>..." to unstage)
  (use "git add <file>..." to mark resolution)
	both modified:   ../moduleBar.h
```
```Bash
# <HEAD> <--- YOU ARE HERE

hash "Support unwibbling of Foo"
hash "Add flibbering support"
hash "Introduce bar module"
hash "Initial commit"
```

The changes belong to the next commit - **`"Introduce FlibberCollection"`**. Lets see the conflict:
```
struct ObjectBar : public FlibberCollection
{
	void splinx();
<<<<<<< HEAD
	void yibble() { myYibble = (myYibble + 1) * 2; }
	void flibber();
=======
	void yibble() { myYibble = myYibble + 1; }
>>>>>>> 1efcbdf (Introduce FlibberCollection)
```

Need to keep the new `yibble()` and drop `flibber()` like in this new commit. It was moved into `FlibberCollection`. This is the updated code:
```C++
struct ObjectBar : public FlibberCollection
{
	void splinx();
	void yibble() { myYibble = (myYibble + 1) * 2; }
```

Now compile and run the project to ensure that it works, then stage the new code and continue the rebase:
```Bash
git add -u
git rebase --continue
```

Oh no, another conflict!
```
Auto-merging moduleBar.h
CONFLICT (content): Merge conflict in moduleBar.h
error: could not apply <hash> Make Bar responsible for to-string
hint: Resolve all conflicts manually, mark them as resolved with
hint: "git add/rm <conflicted_files>", then run "git rebase --continue".
hint: You can instead skip this commit: run "git rebase --skip".
hint: To abort and get back to the state before "git rebase", run "git rebase --abort".
Could not apply <hash> Make Bar responsible for to-string
```

Lets see the state:
```
Changes to be committed:
  (use "git restore --staged <file>..." to unstage)
	modified:   ../main.cpp
	modified:   ../moduleBar.cpp

Unmerged paths:
  (use "git restore --staged <file>..." to unstage)
  (use "git add <file>..." to mark resolution)
	both modified:   ../moduleBar.h
```
```Bash
# <HEAD> <--- YOU ARE HERE

hash "Introduce FlibberCollection"
hash "Support unwibbling of Foo"
hash "Add flibbering support"
hash "Introduce bar module"
hash "Initial commit"
```

The changes belong to the next commit - **`"Make Bar responsible for to-string"`**. Lets see the conflict:
```
struct ObjectBar : public FlibberCollection
{
	void splinx();
<<<<<<< HEAD
	void yibble() { myYibble = (myYibble + 1) * 2; }
=======
	void yibble() { myYibble = myYibble + 1; }
	std::string toString(uint8_t tabs) const;
>>>>>>> 3bddb0e (Make Bar responsible for to-string)
```

Need to keep the new `yibble()` and the `toString()` from the new commit. This is the updated code:
```C++
struct ObjectBar : public FlibberCollection
{
	void splinx();
	void yibble() { myYibble = (myYibble + 1) * 2; }
	std::string toString(uint8_t tabs) const;
```

Now compile and run the project to ensure that it works, then stage the new code and continue the rebase:
```Bash
git add -u
git rebase --continue
```

This one reaches the end of the command list without more conflicts and the rebase is finished. Now you can check that the change is done throughout the whole git log like if it was always there, and there is no trace of the old version of `yibble()`.

---
### Step 8, see commits of specific lines in files
#### *In the end lets inspect with `git-blame` the results.*

Now lets confirm that the specific line that was changed in the previous step was indeed done in one of the first commits, and wasn't touched later.
```Bash
git blame -s moduleBar.h # `-s` is a shorter output.
```
```
hash-A  1) #pragma once
hash-A  2) 
hash-B  3) #include "core.h"
hash-B  4) 
hash-C  5) struct ObjectBar : public FlibberCollection
hash-A  6) {
hash-A  7)    void splinx();
hash-A  8)    void yibble() { myYibble = (myYibble + 1) * 2; }
hash-D  9)    std::string toString(uint8_t tabs) const;
hash-A 10) 
hash-A 11)    std::string mySplinx = "Emptio";
hash-A 12)    uint64_t myYibble = 0;
hash-A 13) };
hash-A 14) 
hash-A 15) ObjectBar makeBar();
```

Can see that `yibble()` was last time changed in commit `hash-A`. Lets see which one is that:
```Bash
git show <hash-A>
```
```
commit hash-A
Author: Vladislav Shpilevoy <v.shpilevoy@tarantool.org>
Date:   <date>

    Introduce bar module
    
    It is able both to splinx and yibble. While for wibble-zorbling the foo
    module is responsible.
```

And apparently it is the commit which was edited on the previous step. All worked as expected.

## Conclusion

Now when the patchset is done, it must be the same as the one at `master` branch. Check the code using `git diff master` while being on your branch. The diff must be empty. Then compare the individual commits.

The branch can be revisited commit-by-commit one last time to check and maybe fix some commit messages and to see if each of them is buildable and runnable. That would make them atomic in a sense that each commit leaves the code in a valid state.

For that make `git rebase -i` to the point just before the first commit that needs checking, and then replace all `pick` with `edit`. Then the git applier will stop at each commit one by one giving you a chance to make changes in them all, where needed.
