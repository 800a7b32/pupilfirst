class UsersController < ApplicationController
  before_filter :authenticate_user!, only: [:show, :edit, :update]
  before_filter :restrict_to_current_user, only: [:show, :edit, :update]

  def show
    @user = User.find(params[:id])
  end

  def edit
    @user = params[:id].present? ? User.find(params[:id]) : current_user
  end

  def update
    @user = User.find(params[:id])
    if @user.update_attributes(user_params)
      redirect_to action: :show
    else
      respond_to :html
    end
  end

  def new
  end

  def invite
    render layout: false
  end

  def send_invite
    @user = User.find_by_email(params[:user][:email]) rescue nil
    if @user.try(:startup).nil?
      @user = User.invite!(invite_params)
      @user.startup = Startup.find(session[:startup_id])
      @user.save!
    else
      @user.errors[:exist] = "this user is associated with other startup"
    end
  end

  def create
  end

  private

  def invite_params
    params.require(:user).permit(:email, :fullname)
  end

  def user_params
      params.require(:user).permit(:salutation,:fullname, :username, :email, :twitter_url, :linkedin_url, :avatar, :startup_id, :title, :born_on, :is_student, :college, :university, :course, :semester )
  end

  def restrict_to_current_user
    raise_not_found if current_user.id != params[:id].to_i
  end
end
