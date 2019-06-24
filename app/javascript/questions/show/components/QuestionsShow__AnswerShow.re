[@bs.config {jsx: 3}];

open QuestionsShow__Types;

let str = React.string;

[@react.component]
let make =
    (
      ~authenticityToken,
      ~answer,
      ~question,
      ~addCommentCB,
      ~currentUserId,
      ~addLikeCB,
      ~removeLikeCB,
      ~userData,
      ~comments,
      ~likes,
      ~handleAnswerCB,
      ~isCoach,
      ~archiveCB,
    ) => {
  let userProfile = userData |> UserData.user(answer |> Answer.creatorId);
  let commentsForAnswer =
    comments |> Comment.commentsForAnswer(answer |> Answer.id);
  let (showAnswerEdit, toggleShowAnswerEdit) = React.useState(() => false);

  let handleCloseCB = () => toggleShowAnswerEdit(_ => false);

  let handleAnswerEditCB = (answer, bool) => {
    toggleShowAnswerEdit(_ => false);
    handleAnswerCB(answer, bool);
  };

  <div className="flex flex-col relative pt-2" key={answer |> Answer.id}>
    {
      showAnswerEdit ?
        <div>
          <QuestionsShow__AnswerEditor
            question
            authenticityToken
            currentUserId
            handleAnswerCB=handleAnswerEditCB
            answer
            handleCloseCB
          />
        </div> :
        <div title={"Answer " ++ (answer |> Answer.id)}>
          <div
            className="max-w-3xl w-full flex mx-auto items-center justify-center relative border shadow bg-white rounded-lg mt-2 z-10">
            <div className="flex w-full">
              <div className="flex flex-col w-full relative">
                <div
                  className="absolute right-0 top-0 flex border border-t-0 border-r-0 border-gray-400 bg-gray-200 rounded-bl">
                  {
                    switch (answer |> Answer.editorId) {
                    | Some(_) =>
                      <a
                        href={
                          "/answers/" ++ (answer |> Answer.id) ++ "/versions"
                        }
                        title="Edit History"
                        className="inline-flex items-center whitespace-no-wrap text-xs font-semibold py-1 px-3 bg-transparent hover:bg-primary-100 hover:text-primary-500 cursor-pointer text-gray-700 border-r border-gray-400">
                        <i className="far fa-history text-sm" />
                        <span className="ml-2"> {"History" |> str} </span>
                      </a>
                    | None => React.null
                    }
                  }
                  {
                    answer |> Answer.creatorId == currentUserId || isCoach ?
                      <div className="flex">
                        <a
                          title="Edit Answer"
                          onClick={_ => toggleShowAnswerEdit(_ => true)}
                          className="inline-flex items-center whitespace-no-wrap text-xs font-semibold py-1 px-3 bg-transparent hover:bg-primary-100 hover:text-primary-500 text-gray-700 border-r border-gray-400 cursor-pointer">
                          <i className="far fa-pen-square text-sm" />
                          <span className="ml-2"> {"Edit" |> str} </span>
                        </a>
                        <QuestionsShow__ArchiveManager
                          authenticityToken
                          id={answer |> Answer.id}
                          resourceType="Answer"
                          archiveCB
                        />
                      </div> :
                      React.null
                  }
                </div>
                <div className="pt-8 pb-4 px-3 md:px-6 flex flex-col">
                  <MarkdownBlock
                    markdown={answer |> Answer.description}
                    className="leading-normal text-sm "
                  />
                  {
                    switch (answer |> Answer.editorId) {
                    | Some(editorId) =>
                      <div>
                        <div
                          className="text-xs mt-1 inline-block px-2 py-1 rounded bg-orange-100 text-orange-900">
                          <span> {"Last edited by " |> str} </span>
                          <span className="font-semibold">
                            {userData |> UserData.userName(editorId) |> str}
                          </span>
                          <span>
                            {
                              " on "
                              ++ (
                                answer
                                |> Answer.updatedAt
                                |> DateTime.stingToFormatedTime(
                                     DateTime.DateWithYearAndTime,
                                   )
                              )
                              |> str
                            }
                          </span>
                        </div>
                      </div>
                    | None => React.null
                    }
                  }
                </div>
                <div
                  className="flex flex-row justify-between items-center pl-2 px-3 md:px-6 pb-4">
                  <div className="pt-4 text-center">
                    <div className="flex flex-row">
                      <QuestionsShow__LikeManager
                        authenticityToken
                        likes
                        answerId={answer |> Answer.id}
                        currentUserId
                        addLikeCB
                        removeLikeCB
                      />
                      <div className="mr-2">
                        <div className="h-10 w-10 p-2">
                          <i
                            className="fal fa-comment-lines text-xl text-gray-600"
                          />
                        </div>
                        <p className="text-xs pb-1">
                          {
                            commentsForAnswer
                            |> List.length
                            |> string_of_int
                            |> str
                          }
                        </p>
                      </div>
                    </div>
                  </div>
                  <QuestionsShow__UserShow
                    userProfile
                    createdAt={answer |> Answer.createdAt}
                    textForTimeStamp="Answered"
                  />
                </div>
              </div>
            </div>
          </div>
          <QuestionsShow__CommentShow
            comments=commentsForAnswer
            userData
            authenticityToken
            commentableType="Answer"
            commentableId={answer |> Answer.id}
            addCommentCB
            currentUserId
            archiveCB
            isCoach
          />
        </div>
    }
  </div>;
};