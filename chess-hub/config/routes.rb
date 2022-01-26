Rails.application.routes.draw do
  root to: "games#index"

  resources :games, only: [:index, :show] do
    post :move
    post :update
  end
end
