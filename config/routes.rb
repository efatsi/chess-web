Rails.application.routes.draw do
  root to: "games#index"

  resources :games, only: [:new, :index, :show] do
    post :move
    post :update
    post :back
    post :change_player

    get :connect, on: :collection
  end
end
